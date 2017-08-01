#include "entity_defs.h"
#include "ricochet.h"
#include "marker.h"
#include "cheats.h"
#include "stats.h"
#include "enemy.h"
#include "particleEffects.h"

SOUND* sndBatDeath = "bat_destroyed.wav";
SOUND* sndBatScream = "bat_scream.wav";


action scream_sprite()
{
	my.alpha = 100;
	vec_set(my.scale_x, vector(0.2, 0.2, 0.2));
	set(my, TRANSLUCENT | BRIGHT);
	var c = 0;
	while(1)
	{
		my.alpha -= 5*time_step;
		c += 0.1*time_step;
		if(my.alpha < 0)
			break;
		vec_set(my.scale_x, vector(0.2 + c * 0.5, 0.2 + c, 0.2 + c));
		wait(1);
	}
	ptr_remove(my);
}


void enemy_bat_fade()
{
	var vDamageDealt = ENEMY_hit(event_type);
}

action enemy_bat()
{
	ENEMY_init();
	my.event = enemy_bat_fade;
	my->material = ObjectMaterial;
	my.skill41 = floatv(49);
	VECTOR myTarget,curTarget;
	vec_set(myTarget,my.x);
	vec_set(curTarget,my.x);
	my.skill31 = random(360);
	my.skill32 = 90+random(20);
	my.skill34 = 19287;
	my.skill35 = 10+random(5);
	static int batId = 0;
	my.skill36 = batId;
	my.skill37 = random(360); // slight pan movement variations
	my.skill60 = 0; // scream timeout
	batId++;
	my.group = 9;
	my.type = TypeBat;
	
	while(my.health > 0)
	{
		if(!(my.eflags&CLIPPED))
		{
			/*my.skill30 += my.skill32*0.1*time_step;
			my.skill30 %= 100;
			ent_animate(my,"idle",my.skill30,0);*/
			my.skill30 += my.skill32*time_step;
			my.skill30 %= 360;
			ent_bonereset_all(my);
			var fac = sinv(my.skill30)*55;
			ent_bonerotate(my,"Bone",vector(fac,fac,fac));
			ent_bonerotate(my,"Bone1",vector(-fac,-fac,-fac));
		}
		
		my.skill37 += 10*time_step;
		my.skill37 %= 360;
		VECTOR temp,temp2,vspeed;
		vec_set(vspeed,nullvector);
		if(player) vec_set(myTarget,player.x);
		myTarget.z = my.z;
		curTarget.z = my.z;
		if(vec_length(vector(myTarget.x-my.x,myTarget.y-my.y,0)) > 32)
		{
			if(LEVEL__stage)
			{
				var flood = stageGetTargetFromFlood(LEVEL__stage,my.x,myTarget,curTarget,FLOOD_PLAYER,20, 1);
				//if(flood > 999) vec_set(curTarget,my.x);
				//draw_num3d(flood,my.x,-80,COLOR_WHITE);
			}
			vec_diff(temp,curTarget,my.x);
			vec_to_angle(temp2,temp);
			my.pan += ang(temp2.x+15*sinv(my.skill37)-my.pan)*0.35*time_step;
			vec_set(temp2,vector(minv(vec_length(temp),my.skill35)*time_step,0,0));
			vec_rotate(temp2,vector(my.pan,0,0));
			vec_set(vspeed,temp2);
			//vec_normalize(temp,minv(vec_length(temp),16)*time_step);
			//vec_add(my.x,temp);
			//draw_line3d2(my.x,curTarget,COLOR_RED,75);
			//draw_line3d2(curTarget,myTarget,COLOR_GREEN,75);
		}	
		my.skill31 += 12*time_step;
		my.skill31 %= 360;
		my.z = 96+16*sinv(my.skill31);
		if(!(my.eflags&CLIPPED))
		{
			vec_set(temp2,nullvector);
			int i = 1;
			you = ent_pvs(i,0); // retrieve first entity
			while (you) // repeat until there are no more entities
			{ 
				if(you != me && your.skill34 == 19287 && my.skill36 > your.skill36)
				{
					vec_diff(temp,my.x,your.x);
					temp.z = 0;
					var len = vec_length(temp);
					if(len < 100)
					{
						vec_normalize(temp,100-len);
						vec_add(temp2,temp);
					}
				}
				i++;
				you = ent_pvs(i,0); // get next entity
			}
			vec_scale(temp2,0.75*time_step);
			vec_add(vspeed,temp2);
		}
		c_move(me,nullvector,vspeed,IGNORE_PASSABLE | IGNORE_SPRITES | IGNORE_PUSH | IGNORE_WORLD);
		if(HIT_TARGET && LEVEL__stage)
		{
			my.skill38 += time_step;
			if(my.skill38 > 16) // reset target if stuck (can happen because of flocking)
			{
				my.skill38 = 0;
				int x,y;
				stageGetIndicesFromPos(LEVEL__stage,my.x,&x,&y);
				vec_set(curTarget,vector(x*200,y*200,0));
			}
		}
		else my.skill38 = 0;
		MARKER_update(me);
		enemy_do_hit_flash(my);
		
		// shoot
		if(my.skill70 == 0)
		{
			if(player != NULL)
			{
				if(vec_dist(player->x, my.x) < 200)
				{
					ENTITY* ent = ent_create("scream.dds", my.x, scream_sprite);
					snd_play(sndBatScream, 100, 0);
					hit_player(6);
					vec_scale(ent->scale_x, 0.5);
					ent->pan = my.pan;
					ent->tilt = 90;
					my.skill70 = 4;
				}
			}
		}
		else 
		{
			my.skill70 = maxv(0, my.skill70 - time_step/16);
		}
		wait(1);
	}
	effect(p_bat_explode,100,my.x,nullvector);
	snd_play(sndBatDeath, 100, 0);
	ptr_remove(me);
	stats_addKill(STATENEMY_BAT);
}

BOOL hex_isDead = 0;
var hex_state = 0;
var hex_loadingWeapon = 0;
action enemy_hex()
{
	ENEMY_init();
	hex_isDead = FALSE;
	ENTITY* laser1 = ent_create("laser.tga", vector(0,0,-999), NULL);
	set(laser1, PASSABLE);
	laser1->material = LaserMaterial;
	laser1.scale_y = 0.5;
	ENTITY* laser2 = ent_create("laser.tga", vector(0,0,-999), NULL);
	set(laser2, PASSABLE);
	laser2.scale_y = 0.5;
	laser2->material = LaserMaterial;
	
	my->material = HexMaterial;
	
	vec_scale(my.scale_x, 0.5);
	while(my.health > 0 && !(cheats_enabled && key_k))
	{
		VECTOR dir;
		vec_diff(dir, player.x, my.x);
		ANGLE angle;
		vec_to_angle(angle, dir);
		my.pan = angle.pan+90.0;
		
		wait(1);
		
		int count = 0;
		for(you = ent_next(NULL); you; you = ent_next(you))
		{
			switch(you.type)
			{
				case TypeEnemy:
				case TypeBat:
				case TypeSphereOfDeath:
					++count;
					break;
			}
		}
		
		if(count == 0) {
			// We don't have enmies in the level anymore
			break;
		}
		
		if(player != NULL) {
			VECTOR tmp;
			tmp.x = player.x;
			tmp.y = player.y;
			tmp.z = my.z;
			var dist = vec_dist(my.x, tmp.x);			
			if(hex_state == 0)
			{
				VECTOR tmp;
				tmp.x = player.x;
				tmp.y = player.y;
				tmp.z = my.z;
				if(dist < 380) {
					hex_loadingWeapon += time_step/16.0;
					if(hex_loadingWeapon > 4)
					{
						hex_loadingWeapon = 0;
						hex_state = 1;
					}
				}
				else {
					hex_loadingWeapon = 0;
				}
			}
			else if(hex_state == 1)
			{
				if(dist > 450) {
					set(laser1, INVISIBLE);
					set(laser2, INVISIBLE);
					hex_state = 0;
				}
				else {
					hit_player(time_step);
					reset(laser1, INVISIBLE);
					reset(laser2, INVISIBLE);
					VECTOR tmp;
					VECTOR eyePos;
					VECTOR targetPos;
					CONTACT c;
					
					vec_set(targetPos.x, player.x);
					targetPos.z -= 40;
					
					// laser 1
					ent_getvertex(my, c, 1170);
					vec_set(eyePos.x, c.x);
					vec_rotate(eyePos.x, my.pan);
					vec_scale(eyePos.x, 0.5);
					vec_add(eyePos.x, my.x);
					
					
					vec_set(tmp.x, eyePos.x);
					var dist = vec_dist(tmp.x, targetPos.x);
					vec_add(tmp.x, targetPos.x);
					vec_scale(tmp.x, 0.5);
					vec_set(laser1.x, tmp.x);
					laser1.skill44 = floatv(dist/128);
					laser1.scale_x = dist/128;
					
					vec_set(tmp.x, eyePos.x);
					vec_sub(tmp.x, targetPos.x);
					
					laser1.pan = 90+atan2v(tmp.y, tmp.x);
					laser1.roll = -atan2v(tmp.z, sqrt(tmp.x*tmp.x + tmp.y*tmp.y));
					
					
					// laser 2
					ent_getvertex(my, c, 718);
					vec_set(eyePos.x, c.x);
					vec_rotate(eyePos.x, my.pan);
					vec_scale(eyePos.x, 0.5);
					vec_add(eyePos.x, my.x);
					
					
					vec_set(tmp.x, eyePos.x);
					var dist = vec_dist(tmp.x, targetPos.x);
					vec_add(tmp.x, targetPos.x);
					vec_scale(tmp.x, 0.5);
					vec_set(laser2.x, tmp.x);
					laser2.skill44 = floatv(dist/128);
					laser2.scale_x = dist/128;
					
					vec_set(tmp.x, eyePos.x);
					vec_sub(tmp.x, targetPos.x);
					
					laser2.pan = 90+atan2v(tmp.y, tmp.x);
					laser2.roll = -atan2v(tmp.z, sqrt(tmp.x*tmp.x + tmp.y*tmp.y));
				}
			}
		}
		
		//FONT_big(100);
		//DEBUG_VAR(count, 16);
	}
	
	hex_isDead = TRUE;
	show_dialog("Room clear!\nTeleporter is now free.");
	
	// effect(p_bat_explode,100,my.x,nullvector);
	// snd_play(sndBatDeath, 100, 0);
	ptr_remove(me);
	
	ptr_remove(laser1);
	ptr_remove(laser2);
}

void enemy_spawn_hex()
{
	VECTOR *pos = stageGetExitPos(LEVEL__stage, NULL, NULL, NULL);
	ent_create("hex_01.mdl", pos, enemy_hex);
}

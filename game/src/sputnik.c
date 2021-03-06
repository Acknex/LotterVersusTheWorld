#include "particleEffects.h"
#include "stats.h"

SOUND* snd_sputnik_attack1 = "sputnik_attack1.wav";
SOUND* snd_sputnik_attack2 = "sputnik_attack2.wav";
SOUND* snd_sputnik_attack3 = "sputnik_attack3.wav";
SOUND* snd_sputnik_death = "sputnik_death.wav";

void sputnik_attack_sound() {
	var i = integer(random(3));
	if(i == 0)
	{
		snd_play(snd_sputnik_attack1, 100, 0);
	}
	else if(i == 1)
	{
		snd_play(snd_sputnik_attack2, 100, 0);
	}
	else if(i == 2)
	{
		snd_play(snd_sputnik_attack3, 100, 0);
	}
}

void enemy_sputnik_fade()
{
	var vDamageDealt = ENEMY_hit(event_type);
}

action enemy_sputnik()
{
	ENEMY_init();
	my.event = enemy_sputnik_fade;
	my->material = ObjectMaterial;
	my.skill41 = floatv(52);
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
	batId++;
	my.group = 9;
	my.type = TypeEnemy;
	my.health = 15; // TANK IT, BABY
	my.damage = 12;
	var attacking = 0;
	var attacked = 0;
	while(my.health > 0)
	{
		my.skill37 += 10*time_step;
		my.skill37 %= 360;
		VECTOR temp,temp2,vspeed;
		vec_set(vspeed,nullvector);
		if(player) vec_set(myTarget,player.x);
		myTarget.z = my.z;
		curTarget.z = my.z;
		
		if(attacking <= 0)
		{
			if(vec_length(vector(myTarget.x-my.x,myTarget.y-my.y,0)) > 125)
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
			//my.skill31 += 12*time_step;
			//my.skill31 %= 360;
			my.z = 64;
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
			c_ignore(30, 0);
			c_move(me,nullvector,vspeed,IGNORE_CONTENT| IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_SPRITES | IGNORE_PUSH | IGNORE_WORLD);
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
			else
			{
				my.skill38 = 0;
			}
		}
		else
		{
			vec_set(vspeed, vector(0,0,0));
		}
		MARKER_update(me);
		
		if(player)
		{
			if(attacking == 0)
			{
				if(vec_dist(vector(player.x, player.y, 0), vector(my.x, my.y, 0)) < 150)
				{
					// Initiate attack
					attacking = time_step;
					sputnik_attack_sound();
				}
			}
			else if(attacking > 0)
			{
				ent_animate(me, "attackA", attacking, 0);
				
				if(attacking >= 30 && attacked == 0)
				{
					var vMode = IGNORE_WORLD | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_ME | SCAN_ENTS;
					c_scan(my.x,my.pan,vector(120, 90, 200), vMode);
					attacked = 1;
				}
				
				if(attacking >= 100)
				{
					attacking = -25;
					attacked = 0;
				}
				attacking += 10 * time_step;
			}
		}
		
		if(attacking <= 0)
		{
			if(vec_length(vspeed) > 0.3) {
				ent_animate(me, "walk", 10 * total_ticks, ANM_CYCLE);
			} else {
				ent_animate(me, "idle", 10 * total_ticks, ANM_CYCLE);
			}
			attacking += time_step;
			if(attacking > 0) { attacking = 0; }
		}
		
		enemy_do_hit_flash(my);
		wait(1);
	}
	effect(p_bat_explode,100,my.x,nullvector);
	snd_play(snd_sputnik_death, 100, 0);
	ptr_remove(me);
	stats_addKill(STATENEMY_SPUTNIK);
}
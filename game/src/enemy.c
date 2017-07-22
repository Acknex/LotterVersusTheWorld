#include "entity_defs.h"
#include "ricochet.h"
#include "marker.h"

#define bulletSpeed skill20
#define bulletLifeTime skill21

#define ENEMY_BULLETLIFETIME 32

void ENEMY__projectileEvent();
void ENEMY__projectileLoop();

SOUND* sndBatDeath = "sounds\\bat_destroyed.wav";

action enemy_projectile()
{
	my->emask |= ENABLE_IMPACT | ENABLE_SHOOT | ENABLE_ENTITY;
	my->bulletSpeed = your->bulletSpeed;
	my->event = ENEMY__projectileEvent;
	my->pan = your->pan;
	my->bulletLifeTime = ENEMY_BULLETLIFETIME;
	vec_set(my->blue, vector(0,0,255));
	set(my, LIGHT);
	my->type = TypeEnemyProjectile;
	ENEMY__projectileLoop();
}


void ENEMY_init()
{
	my->emask |= (ENABLE_SHOOT | ENABLE_SCAN);
	my->health = 3;
	my->damage = 1;
	my->bulletSpeed = 30;
}

var ENEMY_hit(var vEventType)
{
	if (vEventType == EVENT_SHOOT || vEventType == EVENT_SCAN)// || EVENT_IMPACT)
	{
		if (you != NULL)
		{
			if (your->type == TypePlayerProjectile || you == player)
			{
				my->health = maxv(0, my->health - your->damage);
				if (my->health <= 0)
				{
					set (my, dead);
				}
				return your->damage;
			}
		}
	}
	return 0;
}

void ENEMY__projectileEvent()
{
	if ((event_type == EVENT_IMPACT) || (event_type == EVENT_SHOOT))
	{
		if (you != NULL)
		{
			//ignore other enemy shots
			if (your->type != TypeEnemyProjectile)
			{
				set (my, dead);
			}
		}
		else
		{
			set (my, dead);
		}
	}
}

void ENEMY__projectileLoop()
{
	var vFlags = IGNORE_ME | IGNORE_PASSABLE | ACTIVATE_SHOOT;
	while (!is(my, dead))
	{
		my->bulletLifeTime -= time_step;
		VECTOR* to = vector(-12,0,0);
		vec_rotate (to, my->pan);
		vec_add(to, my->x);
		
		//draw_line3d(my->x,NULL,100); // move to first corner   
		//draw_line3d(to,vector(0,0,255),100);
		
		var vDist = c_trace(my.x, to, vFlags);
		if((vDist == 0) && (my->bulletLifeTime > 0)) 
		{
			
			VECTOR* vecMove = vector(-my->bulletSpeed, 0, 0);
			vec_rotate(vecMove, my->pan);
			vec_scale(vecMove, time_step);
			//c_move(me, vector(my->bulletSpeed, 0, 0), nullvector, vFlags);
			vec_add(my->x, vecMove);
		}
		else
		{
			RICOCHET_create(hit.entity);
			set(my, dead);
		}
		//MARKER_update(me);
		wait(1);
	}
	my->event = NULL;
	ptr_remove(me);
}


void enemy_bat_fade()
{
	var vDamageDealt = ENEMY_hit(event_type);
}

action enemy_bat()
{
	ENEMY_init();
	my.event = enemy_bat_fade;
	my->material = LotterMaterial;
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
	my.type = TypeBat;
	
	while(my.health > 0)
	{
		if(!(my.eflags&CLIPPED))
		{
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
				var flood = stageGetTargetFromFlood(LEVEL__stage,my.x,myTarget,curTarget,FLOOD_PLAYER,20);
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
		my.z = 64+16*sinv(my.skill31);
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
		
		wait(1);
	}
	effect(p_bat_explode,100,my.x,nullvector);
	snd_play(sndBatDeath, 100, 0);
	ptr_remove(me);
}

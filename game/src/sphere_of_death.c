#include "enemy.h"
#include "marker.h"

void SPHEREOD__init();
void SPHEREOD__loop();
void SPHEREOD__shoot();
void SPHEREOD__die();
void SPHEREOD__event();
void SPHEREOD__turnOn();

#define sphere_state skill20
#define sphere_anim_open skill21

#define SOD_SEARCH 0
#define SOD_OPEN 1

action enemy_sphere()
{
	set(me, POLYGON);
	set(me, FLAG1);
	my.type = TypeSphereOfDeath;
	
	SPHEREOD__init();
	
	while(me)
	{
		MARKER_update(me);
		wait(1);
	}
}

void SPHEREOD__init()
{
	ENEMY_init();
	my->delayCounter = 0;
	my->event = SPHEREOD__event;
	my->type = TypeSphereOfDeath;
	set(my, PASSABLE | POLYGON | FLAG1);
	
	SPHEREOD__loop();
}

void SPHEREOD__loop()
{
	while(player == NULL)
	{
		wait(1);
	}
	
	// init
	ent_animate(my, "idle", 100, ANM_SKIP);
	VECTOR myTarget,curTarget;
	vec_set(myTarget,my.x);
	vec_set(curTarget,my.x);
	my->sphere_state = SOD_SEARCH;
	
	while (!is(my, dead))
	{
		if(my->sphere_state == SOD_SEARCH)
		{
			if(player) vec_set(myTarget,player.x);
			myTarget.z = my.z;
			curTarget.z = my.z;
			VECTOR temp,temp2,vspeed;
			my.skill35 = 20+random(5);
			my.skill37 = random(360); // slight pan movement variations
			
			var flood = FLOOD_VALUE_MAX;
			if(LEVEL__stage)
			{
				flood = stageGetTargetFromFlood(LEVEL__stage,my.x,myTarget,curTarget,FLOOD_PLAYER,20, 0);
			}
			
			var speed = minv(vec_length(temp),my.skill35)*time_step;
			vec_diff(temp,curTarget,my.x);
			vec_to_angle(temp2,temp);
			my.pan += ang(temp2.x+15*sinv(my.skill37)-my.pan)*0.35*time_step;
			vec_set(temp2,vector(speed,0,0));
			vec_rotate(temp2,vector(my.pan,0,0));
			vec_set(vspeed,temp2);
			my.tilt -= 2*3.1415926*3*speed*time_step;
			
			c_move(me,nullvector,vspeed,IGNORE_PASSABLE | IGNORE_SPRITES | IGNORE_PUSH | IGNORE_WORLD);
			
			int myX, myY;
			int pX, pY;
			stageGetIndicesFromPos(LEVEL__stage, my.x, &myX, &myY);
			stageGetIndicesFromPos(LEVEL__stage, player.x, &pX, pY);
			if(flood < 3 && (pX == myX || pY == myY))
			{
				my->sphere_state = SOD_OPEN;
				my->sphere_anim_open = 0;
				while(my.tilt < 0)
				{
					my.tilt += 360;
				}
			}
		}
		else if(my->sphere_state == SOD_OPEN)
		{
			if(my.tilt > 0)
			{
				my.tilt -= minv(my.tilt, 20*time_step);
				
			}
			else {
				DEBUG_VAR(my->sphere_anim_open, 500);
				my->sphere_anim_open = minv(my->sphere_anim_open + 20*time_step, 100);
				ent_animate(my, "open", my->sphere_anim_open, 0);
			}
		}
		wait(1);	
	}
	SPHEREOD__die();
}

void SPHEREOD_die()
{
	ptr_remove(me);
}

void SPHEREOD__shoot()
{
	/*
	if (my->turretToggle != 584)
		my->turretToggle = 584;
	else
		my->turretToggle = 592;
	
	VECTOR vecPos;
	vec_for_vertex(&vecPos, me, my->turretToggle);
	VECTOR* vecDist = vector(-30, 0, 0);
	ANGLE* angRot = vector(my->shootAngle, 0, 0);
	vec_rotate(vecDist, angRot);
	vec_add (vecDist, &vecPos);
	snd_play(sndTurretShot, 100, 0);
	ENTITY* ent = ent_create(SPHERE_MDL, vecDist, enemy_projectile);	
	ent->pan = my->shootAngle;	
	*/
}

void SPHEREOD__event()
{
	var vDamageDealt = ENEMY_hit(event_type);
}


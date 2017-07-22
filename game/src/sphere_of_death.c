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
#define sphere_anim_up skill22

#define SOD_SEARCH 0
#define SOD_OPEN 1
#define SOD_JUMP 2
#define SOD_CLOSE 3
#define SOD_WAIT 4

SOUND* snd_sphere_activate = "sounds\\sphere_activate.wav";
SOUND* snd_sphere_stomp = "sounds\\sphere_stomp.wav";

void sphere_stomp_effect()
{
	var t = 0;
	my.flags |= LIGHT | PASSABLE | TRANSLUCENT;
	my.red = 255;
	my.green = 128;
	my.blue = 255;
	
	vec_scale(my.scale_x, 0.5);
	wait(1);
	if(my.pan == 0)
		my.pan = 0.01;
	if(my.tilt == 0)
		my.tilt = 0.01;
	my.roll = random(360);
	while(t < 0.3) {
		var f = t / 0.3;
		var scale = 0.01+f*10;
		vec_set(my.scale_x, vector(scale,scale,scale));
		my.alpha = (1-f)*100;
		t += time_step/16;
		DEBUG_VAR(t,150);
		wait(1);
	}
	ptr_remove(my);
}
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
	var base_height = my.z;
	
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
			if(flood != FLOOD_VALUE_MAX)
			{
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
		}
		else if(my->sphere_state == SOD_OPEN)
		{
			if(my.tilt > 0)
			{
				my.tilt -= minv(my.tilt, 20*time_step);
				
			}
			else {
				my->sphere_anim_open = minv(my->sphere_anim_open + 50*time_step, 100);
				ent_animate(my, "open", my->sphere_anim_open, 0);
			}
			if(my->sphere_anim_open == 100)
			{
				my->sphere_state = SOD_JUMP;
				my->sphere_anim_up = 0;
				snd_play(snd_sphere_activate, 30, 0);
			}
		}
		else if(my->sphere_state == SOD_JUMP)
		{
			my->sphere_anim_up = minv(my->sphere_anim_up + .25*time_step, 2);
			my.z = base_height + 100-(my->sphere_anim_up-1)*(my->sphere_anim_up-1)*100;
			if(my->sphere_anim_up == 2)
			{
				my->sphere_state = SOD_CLOSE;
				my->sphere_anim_open = 0;
				snd_play(snd_sphere_stomp, 100, 0);
				ENTITY* ricochet = ent_create("ricochet_blue.tga", vector(my.x, my.y, my.z - 22), sphere_stomp_effect);
				vec_to_angle(ricochet->pan, vector(0, 0, 1));
			}
		}
		else if(my->sphere_state == SOD_CLOSE)
		{
			my->sphere_anim_open = minv(my->sphere_anim_open + 50*time_step, 100);
			ent_animate(my, "open", 100 - my->sphere_anim_open, 0);

			if(my->sphere_anim_open == 100)
			{
				my->sphere_state = SOD_WAIT;
				my->sphere_anim_up = 0;
			}
		}
		else if(my->sphere_state == SOD_WAIT)
		{
			my->sphere_anim_up += time_step/16;
			if(my->sphere_anim_up > 4)
			{
				my->sphere_state = SOD_SEARCH;
			}
		}
		wait(1);	
	}
	SPHEREOD__die();
}

void SPHEREOD__die()
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


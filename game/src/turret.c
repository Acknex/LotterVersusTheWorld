#define TURRET_ATTACKRANGE 500
#define TURRET_SHOOTDELAY 3
#define TURRET_TURNSPEED 5
#define TURRET_ANIMOPENSPEED 5
#define TURRET_ANIMCLOSESPEED 7
#define TURRET_ANIMDIESPEED 8

#define delayCounter skill22
#define animCounter skill23
#define shootAngle skill24
#define turretState skill25
#define turretToggle skill26

#define TURRETOPEN 0
#define TURRETCLOSE 1
#define TURRETACTIVE 2
#define TURRETDIE 3
#define TURRETSLEEP 4

#include "enemy.h"
#include "marker.h"

void TURRET__loop();
void TURRET__shoot();
void TURRET__event();
void TURRET__turnOn();
void TURRET__turnOff();
void TURRET__active();
void TURRET__die();
void TURRET__sleep();


action enemy_turret()
{
	ENEMY_init();
	my->delayCounter = 0;
	my->event = TURRET__event;
	my->type = TypeTurret;
	my->turretState = TURRETSLEEP;
	set(my, PASSABLE);

	TURRET__loop();
}

void TURRET__loop()
{
	while(player == NULL)
	{
		wait(1);
	}
	
	while (!is(my, dead))
	{
		switch(my->turretState)
		{
			case TURRETOPEN:
				TURRET__turnOn();
				break;
			
			case TURRETCLOSE:
				TURRET__turnOff();
				break;
			
			case TURRETACTIVE:
				TURRET__active();
				break;
			
			case TURRETSLEEP:
				TURRET__sleep();
				break;
			
			default:
				break;
		}
		wait(1);	
	}
	TURRET__die();
}

void TURRET__turnOn()
{
	my->animCounter += TURRET_ANIMOPENSPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->turretState = TURRETACTIVE;
		reset(my, PASSABLE);	
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", my->animCounter, 0);	
}

void TURRET__turnOff()
{
	my->animCounter += TURRET_ANIMCLOSESPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->turretState = TURRETSLEEP;	
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", 100 - my->animCounter, 0);
	if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
	{
		my->turretState = TURRETOPEN;
		my->animCounter = 100 - my->animCounter;
	}
}

void TURRET__active()
{
	MARKER_update(me);
	my->delayCounter += time_step;
	if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
	{
		if (my->delayCounter >= TURRET_SHOOTDELAY)
		{
			TURRET__shoot();
		}
		var vTurnStep = TURRET_TURNSPEED * time_step;
		my->shootAngle += vTurnStep;
		ent_bonerotate(me, "Bone1", vector(vTurnStep, 0, 0));
		my->delayCounter = cycle(my->delayCounter, 0, TURRET_SHOOTDELAY);		
	}
	else
	{
		my->turretState = TURRETCLOSE;
		my->animCounter = 0;
	}
}

void TURRET__die()
{
	my->event = NULL;
	my->animCounter = 0;
	
	while (my->animCounter < 100)
	{
		my->animCounter += TURRET_ANIMDIESPEED * time_step;
		my->animCounter = minv(100, my->animCounter);
		ent_animate(me, "die", my->animCounter, 0);	
		ent_bonerotate(me, "Bone1", vector(my->shootAngle, 0, 0));
		wait(1);
	}
	
	set(my, PASSABLE);
	
//	ptr_remove(me);
}

void TURRET__sleep()
{
	if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
	{
		my->turretState = TURRETOPEN;
		my->animCounter = 0;
	}
}

void TURRET__shoot()
{
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
	ENTITY* ent = ent_create(SPHERE_MDL, vecDist, enemy_projectile);	
	ent->pan = my->shootAngle;	
}

void TURRET__event()
{
	var vDamageDealt = ENEMY_hit(event_type);
}


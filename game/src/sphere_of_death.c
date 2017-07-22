#include "enemy.h"
#include "marker.h"

void SPHEREOD__init();
void SPHEREOD__loop();
void SPHEREOD__shoot();
void SPHEREOD__die();
void SPHEREOD__event();
void SPHEREOD__turnOn();

action enemy_sphere()
{
	set(me, POLYGON);
	set(me, FLAG1);
	my.type = TypeSphereOfDeath;
	
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
	
	while (!is(my, dead))
	{
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


#define TURRET_ATTACKRANGE 500
#define TURRET_SHOOTDELAY 8
#define TURRET_TURNSPEED 5

#define delayCounter skill22

#include "enemy.h"
#include "marker.h"

void TURRET__loop();
void TURRET__shoot();
void TURRET__event();


action enemy_turret()
{
	my->scale_x *= 2;
	my->scale_y *= 2;
	my->scale_z *= 5;
	ENEMY_init();
	my->delayCounter = 0;
	my->event = TURRET__event;
	my->type = TypeTurret;

	TURRET__loop();
}

void TURRET__loop()
{
	while (!is(my, dead))
	{
		MARKER_update(me);
		my->delayCounter += time_step;
		if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
		{
			if (my->delayCounter >= TURRET_SHOOTDELAY)
			{
				TURRET__shoot();
			}
			
		}
		my->pan += TURRET_TURNSPEED * time_step;
		my->delayCounter = cycle(my->delayCounter, 0, TURRET_SHOOTDELAY);
		wait(1);	
	}
	ptr_remove(me);
}

void TURRET__shoot()
{
	VECTOR* vecDist = vector(30, 0, 0);
	vec_rotate(vecDist, my->pan);
	vec_add (vecDist, my->x);
	ENTITY* ent = ent_create(SPHERE_MDL, vecDist, enemy_projectile);	
	ent->pan = my->pan;
	
}

void TURRET__event()
{
	var vDamageDealt = ENEMY_hit(event_type);
}


#include "entity_defs.h"
#include "ricochet.h"
#include "marker.h"

#define bulletSpeed skill20
#define bulletLifeTime skill21

#define ENEMY_BULLETLIFETIME 32

void ENEMY__projectileEvent();
void ENEMY__projectileLoop();

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
		
		draw_line3d(my->x,NULL,100); // move to first corner   
		draw_line3d(to,vector(0,0,255),100);
		
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
		MARKER_update(me);
		wait(1);
	}
	my->event = NULL;
	ptr_remove(me);
}


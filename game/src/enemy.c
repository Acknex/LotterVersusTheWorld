#include "entity_defs.h"

#define bulletSpeed skill20

void ENEMY__projectileEvent();
void ENEMY__projectileLoop();

action enemy_projectile()
{
	my->emask |= ENABLE_IMPACT | ENABLE_SHOOT | ENABLE_ENTITY;
	my->bulletSpeed = your->bulletSpeed;
	my->event = ENEMY__projectileEvent;
	my->pan = your->pan;
	set(my, FLAG2);
	my->push = -1;
	ENEMY__projectileLoop();
}


void ENEMY_init()
{
	my->emask |= ENABLE_SHOOT;
	my->z -= my->min_z;
	my->health = 3;
	my->damage = 1;
	my->bulletSpeed = 5;
}

var ENEMY_hit(var vEventType)
{
	if (vEventType == EVENT_SHOOT || EVENT_IMPACT)
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
	error("MUH");
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
	else
	{
		error("kacke");
	}
}

void ENEMY__projectileLoop()
{
	var vFlags = IGNORE_FLAG2 | ACTIVATE_SHOOT;
	while (!is(my, dead))
	{
		
		c_move(me, vector(my->bulletSpeed, 0, 0), nullvector, vFlags);
		wait(1);
	}
	ptr_remove(me);
}


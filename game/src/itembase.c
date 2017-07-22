/*
 *******************************************************************************
 * itembase.c
 * Creation date: 08.06.2013
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2011-02-23 22:27:12 +0100 (Mi, 23 Feb 2011) $
 * $Revision: 6 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for collectible items
 *
 * Comments
 * 
 * for short descriptions see comments in items.h
 *
 *******************************************************************************
 */

#include <mtlfx.c>
#include <particles.c>
#include "marker.c"

#define PARTICLECNT skill42

SOUND* sndCollect = "collect.wav";

void ITEM__particleFader(PARTICLE *p);
void ITEM__particle (PARTICLE *p); 


void ITEM_setup()
{
	my->emask |= ENABLE_TRIGGER; 
	my->trigger_range = 20;
	set (me, PASSABLE);
}

void ITEM_fade()
{
	var vTicks = total_ticks;
	VECTOR vecDist;
	
	vec_set(&vecDist, &my->x);
	vec_sub(&vecDist, &player->x);
	while (my->scale_x > 0)
	{
		//scale down item
		my->scale_x = maxv(my->scale_x - 0.1 * time_step, 0);
		my->scale_y = my->scale_x;
		my->scale_z = my->scale_x;
		my->pan += (total_ticks - vTicks) * 10 * time_step;

		//move item towards player
		vec_set(&my->x, &vecDist);
		vec_scale(&my->x, my->scale_x);
		vec_add(&my->x, &player->x);
		wait(1);
	}
	ptr_remove(me);
}

void ITEM_collect()
{
	my->event = NULL;
	set (me, is_collected);
	ent_playsound(me, sndCollect, 1000);
}

void ITEM_update()
{
	MARKER_update(me);
	my->PARTICLECNT += time_step;
	if (my->PARTICLECNT > 10)
	{
		effect(ITEM__particle, 10, &my->x, nullvector);
		my->PARTICLECNT -= 10;
	}
}

void ITEM__particleFader(PARTICLE *p) 
{
	p->alpha -= 5 * time_step;
	p->size += time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
}

void ITEM__particle (PARTICLE *p) 
{
	VECTOR vecTemp;
	vec_randomize(&vecTemp, 10);
	vec_normalize(&vecTemp, 4);
	vec_add (&p->vel_x, &vecTemp);
	p->vel_y = 0;
	vec_set(&p->blue, vector(0, 255, 255));
	set(p, MOVE | TRANSLUCENT | BRIGHT);
	p->lifespan = 80;
	p->size  = 30 + random(5);
	p->event = ITEM__particleFader;
}


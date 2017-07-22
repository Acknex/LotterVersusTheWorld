/*
 *******************************************************************************
 * items.c
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
#include "itembase.h"

#define PARTICLECNT skill42


SOUND* sndCollect = "collect.wav";

void item_coin_evt();
void item_life_evt();
void item_jetpack_evt();


action item_coin()
{
	ITEM_setup();	
	my->event = item_coin_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	my->material = mtl_specBump;
	my->pan = random(360);
	
	while(!is(me, is_collected))
	{
		ITEM_update();
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan -= 5 * time_step;
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
	ITEM_fade();
}

action item_life()
{
	ITEM_setup();
	my->event = item_life_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	
	while(!is(me, is_collected))
	{
		ITEM_update();
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan += 20 * time_step * sinv(total_ticks * 2 - vOffset);
		wait(1);
	}
	ITEM_fade();
}


action item_jetpack()
{
	ITEM_setup();
	my->event = item_jetpack_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	
	while(!is(me, is_collected))
	{
		ITEM_update();
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan = 135 * sinv(total_ticks * 2 - vOffset);
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
	ITEM_fade();
}

void item_coin_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
	}
}

void item_life_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
	}
}

void item_jetpack_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
	}
}

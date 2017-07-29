
#define HOLE_ATTACKRANGE 500
#define HOLE_ANIMOPENSPEED 7
#define HOLE_ANIMCLOSESPEED 7
#define HOLE_ANIMACTIVESPEED 20

#define holeState skill25
#define entryTicks skill27

#define HOLEOPEN 0
#define HOLECLOSE 1
#define HOLEACTIVE 2
#define HOLESLEEP 3

#include "marker.h"
#include "entity_defs.h"

void HOLE__loop();
void HOLE__turnOn();
void HOLE__turnOff();
void HOLE__active();
void HOLE__sleep();
void HOLE__shoot();

SOUND* sndHoleOn = "spikes_up.wav";


action enemy_hole()
{
	set(me, POLYGON | PASSABLE | FLAG1);
	my.type = TypeHole;
	my.material = ObjectMaterial;
	my.skill41 = floatv(26);
	ent_animate(my, "idle", 0, 0);
	my->holeState = HOLESLEEP;
	my->damage = 0;
		
	HOLE__loop();
	
	/* old
	while(1)
	{
		my.skill41 = floatv(0.5 + 0.5 * sinv(total_ticks));
		ent_animate(me, "activate", total_ticks * 2, ANM_CYCLE);
		MARKER_update(me);
		HOLE__shoot();
		
		wait(1);
	}*/
}

void HOLE__loop()
{
	while(player == NULL)
	{
		wait(1);
	}
	
	while (1)
	{
		switch(my->holeState)
		{
			case HOLEOPEN:
				HOLE__turnOn();
				break;
			
			case HOLECLOSE:
				HOLE__turnOff();
				break;
			
			case HOLEACTIVE:
				HOLE__active();
				break;
			
			case HOLESLEEP:
				HOLE__sleep();
				break;
			
			default:
				break;
		}
		wait(1);	
	}
}

void HOLE__turnOn()
{
	MARKER_update(me);
	my->animCounter += HOLE_ANIMOPENSPEED * time_step;
	if (my->animCounter >= 60)
	{
		//make sure animation switches smoothly
		my->entryTicks = total_ticks;
		my->holeState = HOLEACTIVE;
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "activate", my->animCounter, 0);
	my->skill41 = floatv(0.01 * my->animCounter);
}

void HOLE__turnOff()
{
	MARKER_update(me);
	my->animCounter += HOLE_ANIMCLOSESPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->holeState = HOLESLEEP;	
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "activate", 100 - my->animCounter, 0);
	if (vec_dist(player->x, my->x) < HOLE_ATTACKRANGE)
	{
		my->holeState = HOLEOPEN;
		snd_play(sndHoleOn, 100, 0);
		my->animCounter = 100 - my->animCounter;
	}
	my->skill41 = floatv(0.01 * (100 - my->animCounter));
}

void HOLE__sleep()
{
	my->skill41 = floatv(0.0);
	if (vec_dist(player->x, my->x) < HOLE_ATTACKRANGE)
	{
		my->holeState = HOLEOPEN;
		snd_play(sndHoleOn, 100, 0);
		my->animCounter = 0;
	}
}

void HOLE__active()
{
	MARKER_update(me);
	my->delayCounter += time_step;
	if (vec_dist(player->x, my->x) < HOLE_ATTACKRANGE)
	{
		HOLE__shoot();
		//toggle between 60 an 100, start with 60 (open state left with 60)
		my->animCounter = 100 - (20 * cosv((total_ticks - my->entryTicks) * HOLE_ANIMACTIVESPEED) + 20);
		my->skill41 = floatv(0.01 * my->animCounter);
		ent_animate(me, "activate", my->animCounter, 0);
	}
	else
	{
		my->holeState = HOLECLOSE;
		my->animCounter = 100 - my->animCounter ;
	}
}

void HOLE__shoot()
{
	var vMode = IGNORE_WORLD | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_ME | SCAN_ENTS;
	VECTOR vecTemp;
	vec_set(&vecTemp, my->x);
	vecTemp.z += 50;
	c_scan(&vecTemp,nullvector,vector(360, 0, 180), vMode);
}
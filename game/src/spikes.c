#define SPIKES_ATTACKRANGE 500
#define SPIKES_ANIMOPENSPEED 10
#define SPIKES_ANIMCLOSESPEED 7
#define SPIKES_ANIMOPENSPEEDFAST 40
#define SPIKES_ANIMCLOSESPEEDFAST 20
#define SPIKES_ALTERNATIONTIME 64
#define SPIKES_COOLDOWN 8

#define spikesState skill25
#define spikesCoolDown skill26

#define SPIKESOPEN 0
#define SPIKESCLOSE 1
#define SPIKESACTIVE 2
#define SPIKESSLEEP 3

#include "marker.h"
#include "entity_defs.h"

void SPIKES__loop();
void SPIKES__turnOn();
void SPIKES__turnOff();
void SPIKES__active();
void SPIKES__sleep();
void SPIKES__shoot();

SOUND* sndSpikesUp = "spikes_up.wav";

action enemy_spikes()
{
  	set(me, POLYGON | FLAG1 | PASSABLE);
	my.type = TypeSpikes;
	ent_animate(my, "idle", 0, 0);
	my->spikesState = SPIKESSLEEP;
	my->trigger_range = 30;
	my->event = NULL;
	my->damage = 5;
	
	SPIKES__loop();
	
}

void SPIKES__loop()
{
	while(player == NULL)
	{
		wait(1);
	}
	
	while (1)
	{
		switch(my->spikesState)
		{
			case SPIKESOPEN:
				SPIKES__turnOn();
				break;
			
			case SPIKESCLOSE:
				SPIKES__turnOff();
				break;
			
			case SPIKESACTIVE:
				SPIKES__active();
				break;
			
			case SPIKESSLEEP:
				SPIKES__sleep();
				break;
			
			default:
				break;
		}
		wait(1);	
	}
}

void SPIKES__turnOn()
{
	MARKER_update(me);
	my->animCounter += SPIKES_ANIMOPENSPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->delayCounter = 0;
		my->spikesState = SPIKESACTIVE;
		my->spikesCoolDown = SPIKES_COOLDOWN;
		//reset(my, PASSABLE);	
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", my->animCounter, 0);
	my->skill41 = floatv(0.01 * my->animCounter);
}

void SPIKES__turnOff()
{
	MARKER_update(me);
	my->animCounter += SPIKES_ANIMCLOSESPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->spikesState = SPIKESSLEEP;	
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", 100 - my->animCounter, 0);
	if (vec_dist(player->x, my->x) < SPIKES_ATTACKRANGE)
	{
		my->spikesState = SPIKESOPEN;
		snd_play(sndSpikesUp, 100, 0);
		my->animCounter = 100 - my->animCounter;
	}
	my->skill41 = floatv(0.01 * (100 - my->animCounter));
}

void SPIKES__sleep()
{
	//MARKER_update(me);
	my->skill41 = floatv(0.0);
	if (vec_dist(player->x, my->x) < SPIKES_ATTACKRANGE)
	{
		my->spikesState = SPIKESOPEN;
		snd_play(sndSpikesUp, 100, 0);
		my->animCounter = 0;
	}
	//my->skill41 = floatv(clamp(0.3 * sinv(10 * total_ticks), 0, 1));
}

void SPIKES__active()
{
	MARKER_update(me);
	my->delayCounter += time_step;
	if (vec_dist(player->x, my->x) < SPIKES_ATTACKRANGE)
	{
		my->delayCounter = cycle(my->delayCounter + time_step, 0, SPIKES_ALTERNATIONTIME);
		if (my->delayCounter > SPIKES_ALTERNATIONTIME * 0.5)
		{
			my->animCounter -= SPIKES_ANIMCLOSESPEEDFAST * time_step;
			my->animCounter = maxv(0, my->animCounter);
			my->spikesCoolDown = 0;
		}
		else
		{
			my->animCounter += SPIKES_ANIMOPENSPEEDFAST * time_step;
			if (my->animCounter >= 50)
			{
				my->spikesCoolDown += time_step;
				if (my->spikesCoolDown > SPIKES_COOLDOWN)
				{
					SPIKES__shoot();
					my->spikesCoolDown -= SPIKES_COOLDOWN;
				}
			}
			my->animCounter = minv(100, my->animCounter);
			
		}
		my->skill41 = floatv(0.01 * my->animCounter);
		ent_animate(me, "open", my->animCounter, 0);
	}
	else
	{
		my->spikesState = SPIKESCLOSE;
		my->animCounter = 100 - my->animCounter ;
	}
}

void SPIKES__shoot()
{
	var vMode = IGNORE_WORLD | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_ME | SCAN_ENTS;
	VECTOR vecTemp;
	vec_set(&vecTemp, my->x);
	vecTemp.z += 50;
	c_scan(&vecTemp,nullvector,vector(360, 0, 180), vMode);
}
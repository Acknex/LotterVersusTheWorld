
#define textTimer skill20
#define TEXTDURATION 80

var QUEST__solved = 0;
var QUEST__started = 0;

var QUEST__count = 5;
var QUEST__id = 0;

void QUEST__masterEvent();
void QUEST__droptofloor();

#include "entity_defs.h"
#include "materials.h"
#include "itembase.h"
#include "teleporter.h"
#include "camera.h"
#include "marker.h"
#include "util.h"

SOUND* sndQuestStarted = "turret_up.wav";
SOUND* sndQuestDone = "turret_down.wav";

TEXT* txtQuestTasks = 
{
	strings = 5;
		
	string(	
	/* 0 */ "Find the McGuffin",
	/* 1 */ "Locate nexus module",
	/* 2 */ "Obtain Acknex2 sources",
	/* 3 */ "Retreive malware kit",
	/* 4 */ "Pick up Stuxnet remains"
	);
}

STRING * strQuestMessage = "#256";

action questarrow()
{
	set(my, INVISIBLE | PASSABLE);
	my->material = HologramMaterial;
	if (you== NULL)
	{
		wait(1);
		ptr_remove(me);
		return;
	}
	ENTITY* questmaster = you;
	while(player == NULL)
	{
		wait(1);
	}
	reset(my, INVISIBLE);
	VECTOR diff;
	ANGLE angle;
	var angRoll = 0;
	VECTOR* vecDist;
	var dist;
	var distFac = 0;
	while(QUEST__started == 0 && questmaster != NULL)
	{
		vec_diff(&diff, questmaster->x, player->x);
		vec_to_angle(&angle, &diff);
		angRoll = ang(angRoll + 15* time_step);
		vec_set(my->pan, vector(angle.pan, 0, angRoll));	
		dist = vec_length(&diff);
		if (dist < 600)
		{
			distFac = cycle(distFac + time_step, 0, 10);
		}
		else
		{
			if (distFac == 10)
				distFac = 0;
				
			if (distFac != 0)
				distFac = minv(distFac + time_step, 10);
		}
		var distFac01 = distFac * 0.1;
		dist = (maxv((dist - 100), 100) * distFac01) + (100 * (1 - distFac01));
		vecDist = vector(dist, 0, 0);
		vec_rotate(vecDist, vector(my->pan, 0 , 0));
		vec_add(vecDist, player->x);
		vecDist->z -= 80;
		vec_set(my->x, vecDist);

		wait(1);	
	}
	VECTOR timeVec;
	while(my->scale_x > 0)
	{
		wait (1);
		vec_fill(&timeVec, 0.5 * time_step);
		vec_sub(my->scale_x, timeVec);		
	}
	vec_fill(my->scale_x, 0);
	ptr_remove(me);
}

action questmaster()
{
	my->type = TypeQuestmaster;
	vec_scale(my->scale_x, 2.5);
	wait(1);
	//my->event = QUEST__masterEvent;
	my->material = HologramMaterial;
	//my->emask |= ENABLE_TRIGGER;
	//my->trigger_range = 80;
	my->z = 150;
	QUEST__droptofloor();
	set(me, PASSABLE);
	while(player == NULL)
	{
		wait(1);
	}
	ent_create("arrow.mdl", my->x, questarrow);
	while(QUEST__started == 0)
	{
		MARKER_update(me);
		VECTOR vecTemp;
		if ((vec_dist2d(my.x, player.x) < 80) || (cheats_enabled && key_q)) // We collided with it ;)
		{
			QUEST__started = 1;
			set(my, PASSABLE);		
			snd_play(sndQuestStarted, 100, 0);
		
			str_cpy(strQuestMessage, "New Mission obtained:\n");
			str_cat(strQuestMessage, (txtQuestTasks->pstring)[QUEST__id]);
			show_dialog(strQuestMessage);
		}
		else
		{
			vec_set(vecTemp, player->x);
			vec_sub(vecTemp, my->x);
			ANGLE angTemp;
			vec_to_angle(angTemp, vecTemp);
			my->pan = my->pan*0.3 + angTemp.pan*0.7;
		}
		wait(1);
		my->animCounter = cycle(my->animCounter += 2 * time_step,0,100);
		ent_animate(me, "stand", my->animCounter, ANM_CYCLE);
		my.alpha=cycle(my.alpha+5*time_step,0,100);
	}
	wait(-2);
	while(my->scale_z > 0)
	{
		MARKER_update(me);
		wait(1);
		my->scale_z -= 0.15 * time_step;
		QUEST__droptofloor();
	}
	while(my->scale_x > 0)
	{
		MARKER_update(me);
		wait(1);
		my->scale_x -= 0.25 * time_step;
		my->scale_y -= 0.25 * time_step;
	}
	vec_set(my->scale_x, nullvector);
	wait(1);
	ptr_remove(me);
}

action questitem()
{
	set(my, PASSABLE | INVISIBLE);//hack
	vec_scale(my->scale_x, 2.5);
	my->type = TypeQuestitem;
	my->z = 80;
	switch (QUEST__id)
	{
		case 0:
			ent_morph(me, "jetpack_lotter.mdl");
			break;
			
		case 1:
			ent_morph(me, "item_a3_collectable.mdl");
			break;
			
		case 2:
			ent_morph(me, "tronding1.mdl");
			break;
			
		case 3:
			ent_morph(me, "gocart01.mdl");
			break;
			
		case 4:
			ent_morph(me, "lottifant.mdl");
			break;
			
	}
	set(my, TRANSLUCENT);
	//my->alpha = 0;
	my->material = HologramMaterial;
	//my->emask |= ENABLE_TRIGGER;
	//my->trigger_range = 80;
	set(me, INVISIBLE);
	while (QUEST__started == 0)
	{
		wait(1);
	}
	reset(my, INVISIBLE);//hack
	while (my->alpha < 100)
	{
		MARKER_update(me);
		wait(1);
		my->alpha += 10* time_step;
	}
	my->alpha = 100;
	reset(my, TRANSLUCENT);
	


	//var vZ = my->z;
	var vOffset = random(500);
	var showedMessage = 0;
	var countDown = 6;
	while(countDown > 0)
	{
		//my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan = 135 * sinv(total_ticks * 2 - vOffset);
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		VECTOR vecTemp;
		
		if ((vec_dist2d(my.x, player.x) < 80) || (cheats_enabled && key_q)) // We collided with it ;)
		{
			set(me, is_collected);
		}

		if (is(me, is_collected))
		{
			countDown -= time_step;
		}
		
		MARKER_update(me);
		wait(1);
	}

	snd_play(sndQuestDone, 100, 0);

	var smoothTransition = 10;
	var hasTriggered = 0;
	var angle = 0;
	var newPan = 0;
	VECTOR* dist;
	VECTOR myPos;
	VECTOR myScale;
	VECTOR myRotation;
	vec_set(&myPos, my->x);
	vec_set(&myScale, my->scale_x);
	vec_set(&myRotation, my->pan);
	while(player != NULL)
	{
		angle += 10 * time_step;
		dist = vector(80, 0, 0);	
		vec_rotate(dist, vector(angle, 0, 0));
		vec_add(dist, player->x);
		dist->z -= 80;
		newPan -= 15* time_step;
		smoothTransition = clamp(smoothTransition - time_step, 0, 10);
		if (smoothTransition <= 0) 
		{
			if (hasTriggered == 0)
			{
				hasTriggered = 1;
				str_cpy(strQuestMessage, "Mission completed!\nTeleporter enabled.");
				show_dialog(strQuestMessage);
			
				QUEST__solved = 1;
				teleporter_enable(); //switch to end boss?
			}
			vec_set(my->x, dist);
			my->pan = newPan;
		}
		else
		{
			vec_lerp(my->x, dist, &myPos, smoothTransition / 10);
			vec_lerp(my->scale_x, vector(1, 1, 1), &myScale, smoothTransition / 10);
			vec_lerp(my->pan, vector(newPan, 0, 0), &myRotation, smoothTransition / 10);
		}
		wait(1);
	}	
	
	ptr_remove(me);
}


void QUEST_reset()
{
	QUEST__solved = 0;
	QUEST__started = 0;
}

void QUEST_init()
{
	QUEST_reset();
	QUEST__id = integer(random(5));
}

var QUEST_isSolved()
{
	return QUEST__solved;
}

var QUEST_isStarted()
{
	return QUEST__started;
}

void QUEST__droptofloor()
{
	VECTOR to;
	vec_set(to, my->x);
	to.z -= 300;
	var dist = c_trace(my->x, to, IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_SPRITES);
	if (dist > 0)
	{
		my.z -= dist+my->min_z;
	}
}

#define textTimer skill20
#define TEXTDURATION 64

var QUEST__solved = 0;
var QUEST__started = 0;

var QUEST__count = 5;
var QUEST__id = 0;

void QUEST__masterEvent();
void QUEST__itemEvent();

#include "entity_defs.h"
#include "materials.h"
#include "itembase.h"
#include "teleporter.h"
#include "camera.h"

TEXT* txtQuestTasks = 
{
	strings = 5;
		
	string(	
	/* 0 */  "Grab a jetpack",
	/* 1 */ 	"Compile with Acknex4",
	/* 2 */  "Obtain Acknex2 sources",
	/* 3 */  "Bring me a racecar",
	/* 4 */  "Where's my Lottifant?"
	);
}

TEXT* txtQuestModels = 
{
	strings = 5;
		
	string(	
	/* 0 */  "jetpack.mdl",
	/* 1 */ 	"Compile with Acknex4",
	/* 2 */  "Obtain Acknex2 sources",
	/* 3 */  "Bring me a racecar",
	/* 4 */  "Where's my Lottifant?"
	);
}

action questmaster()
{
	vec_scale(my->scale_x, 3);
	my->event = QUEST__masterEvent;
	my->material = HologramMaterial;
	while(1)
	{
		if (is(me, is_collected))
		{
			if (my->textTimer < TEXTDURATION)
			{
				VIEW* view = get_camera();
				my->textTimer += time_step;
				VECTOR vecTemp;
				vec_set(&vecTemp, my->x);
				if ((vec_to_screen(vecTemp, get_camera()) != NULL))
				{
					STRING* str = (txtQuestTasks->pstring)[QUEST__id];
					draw_text(str, vecTemp.x, vecTemp.y, vector(255,255,255));
				}
			}
		}
		wait(1);
		my->animCounter = cycle(my->animCounter += 5 * time_step,0,100);
		ent_animate(me, "idle", my->animCounter, ANM_CYCLE);
	}
}

action questitem()
{
	set(my, TRANSLUCENT);
	my->alpha = 0;
	my->material = HologramMaterial;
	while (QUEST__started == 0)
	{
		wait(1);
	}
	while (my->alpha < 100)
	{
		wait(1);
		my->alpha += 10* time_step;
	}
	my->alpha = 100;
	reset(my, TRANSLUCENT);
	my->event = QUEST__itemEvent;
	
	while(!is(me, is_collected))
	{
		wait(1);
	}
	QUEST__solved = 1;
	ITEM_fade();
	teleporter_enable();
}

void questspawn()
{
	ent_create("warlock.mdl", player.x, questmaster);
}
void my_startup(){on_x= questspawn;}


void QUEST_init()
{
	QUEST__id = integer(random(5));
}

void QUEST_reset()
{
	QUEST__solved = 0;
	QUEST__started = 0;
}

var QUEST_isSolved()
{
	return QUEST__solved;
}

void QUEST__masterEvent()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
		QUEST__started = 1;
	}
}

void QUEST__itemEvent()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
	}
}

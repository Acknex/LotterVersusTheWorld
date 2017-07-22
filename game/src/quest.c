
#define textTimer skill20
#define TEXTDURATION 64

var QUEST__solved = 0;
var QUEST__started = 0;

var QUEST__count = 5;
var QUEST__id = 0;

#include "entity_defs.h"
#include "materials.h"

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
	my->event = QUEST__masterEvent;
	my->material = HologramMaterial;
	while(!is(me, collected))
	{
		if (is(me, collected)
		{
			if (textTimer < TEXTDURATION)
			{
				VIEW* view = get_camera();
				textTimer += time_step;
				VECTOR vecTemp;
				vec_set(&vecTemp, ent->x);
				if ((vec_to_screen(vecTemp, get_camera()) != NULL) && (ent->type < txtMarkers->strings))
				{
					STRING* str = (txtMarkers->pstring)[ent->type];
					draw_text(str, vecTemp.x, vecTemp.y, vector(255,255,255));
				}
			}
		}
		wait(1);
		my->animSpeed = cycle(my->animSpeed += 5 * time_step,0,100);
		ent_animate(me, "attack", my->animSpeed, ANM_CYCLE);
	}
}

action questitem()
{
	set(my, TRANSLUCENT);
	my->alpha = 0;
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
	
	while(!is(me, collected))
	{
		wait(1);
	}
	QUEST__solved = 1;
	ITEM_fade();
}

QUEST_init()
{
	QUEST__id = integer(random(5));
}

QUEST_reset()
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

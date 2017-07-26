#include "camera.h"
#include "marker_cfg.h"
#include "entity_defs.h"

var MARKER__show = 0;
var MARKER__enable = 0;
FONT * MARKER_font;

typedef struct MARKERdata
{
	ENTITY * owner;
	ENTITY * sprite;
	BMAP * target;
	TEXT * text;
	STRING * contents;
	int countdown;
} MARKERdata;

var activeMarkers = 0;
/*void MARKER_debug_startup()
{
	while(1)
	{
		DEBUG_VAR(activeMarkers, 150);
		wait(1);
	}
}*/


function MARKER_mtlRemove_event()
{
	return 1;
}

MATERIAL * MARKER_mtlRemove = 
{
	event = MARKER_mtlRemove_event;
	flags = ENABLE_RENDER;
}

function MARKER_entRemove(ENTITY * ent)
{
	if(ent == NULL) {
		return;
	}
	if (ent->type != TypeMarker)
		return;
	
	activeMarkers--;
	MARKERdata * data = (void*)ent.markerData;
	if(data == NULL) {
		return;
	}

	ptr_remove(data.text);
	ptr_remove(data.contents);
	ptr_remove(data.target);
	
	ENTITY * other = NULL;
	if(ent == data.owner) {
		other = data.sprite;
	} else {
		other = data.owner;
	}
	
	free(data);
	
	if(other != NULL) {
		other.markerData = NULL;
		//ent_remove(other);
	}
}

function MARKER_init()
{
	MARKER_font = font_create("monoid#64");
	//MARKER_entRemoveLink = on_ent_remove;
	on_ent_remove = MARKER_entRemove;
}


action MARKER_sprite()
{
	activeMarkers++;
	var killme = 0;
	my.type = TypeMarker;
	my.material = MARKER_mtlRemove;
	my.skill41 = floatv(5);
	set(me, PASSABLE | DECAL);
	wait(1); // Wait for initialization
	
	proc_mode = PROC_LATE;
	
	vec_set(my.pan, vector(180, 90, 0));
	vec_fill(my.scale_x, 0.6);
	
	while(my.markerData != NULL && killme == 0)
	{
		MARKERdata * data = (void*)my.markerData;
		if(data != NULL)
		{
			vec_set(my.x, data.owner.x);
			my.z = 0;
			my.y -= my.scale_y * 0.5 * bmap_width(data.target);
			my.y += data.owner.min_y;
		
			if(data.countdown > 0)
			{
				reset(me, INVISIBLE);
				data.countdown --;
				
				// Clear the render target, so we don't have overwrite
				bmap_rendertarget(data.target, 0, 0);
				draw_quad(
					NULL,
					NULL, NULL,
					vector(bmap_width(data.target), bmap_height(data.target), 0), NULL,
					COLOR_BLACK,
					100, 0);
				bmap_rendertarget(NULL, 0, 0);
			}
			else
			{
				set(me, INVISIBLE);
				//kill entity in order to stop expensive rendertarget processing
				//sprite will be recreated automatically, this is less expensive
				killme = 1;
			}
		}
		
		wait(1);
	}
	ent_remove(me);
}

function MARKER_initializeEntity(ENTITY * ent)
{
	if(ent.markerData != 0) {
		return;
	}
	
	ent.markerData = 10;
	
	MARKERdata * data = malloc(sizeof(MARKERdata));
	
	data.target = bmap_createblack(512, 256, 8888);
	data.text = txt_create(1, -2);
	data.owner = ent;
	data.sprite = ent_create("ICON.png", ent.x, MARKER_sprite);
	
	ent_clone(data.sprite);
	ent_setskin(data.sprite, data.target, 1);
	
	data.contents = (data.text.pstring)[0];
	data.text.target_map = data.target;
	data.text.font = MARKER_font;
	data.text.flags = SHOW | LIGHT;
	switch(ent.type)
	{
		case TypePlayer:
		case TypeQuestmaster:
		case TypeQuestitem:
		case TypeTeleporterDisabled:
		case TypeTeleporterEnabled:
			// UI Color 1
			vec_set(data.text.blue, COLOR_RED);
			break;
		default:
			// UI Color 2
			vec_set(data.text.blue, COLOR_GREEN);
			break;
	}
	
	data.sprite.markerData = (void*)data;
	data.owner.markerData = (void*)data;
	
	str_cpy(data.contents, "<ERROR>");
}

void MARKER_toggle()
{
	MARKER__show = 1 - MARKER__show;
}

void MARKER_attach()
{
	MARKER__enable = 1;
}

void MARKER_detach()
{
	MARKER__enable = 0;
}

void MARKER_setText(ENTITY * ent, STRING * text)
{
	if (MARKER__show == 0 || MARKER__enable == 0)
		return;
	
	if (ent == NULL || player == NULL)
	{
		return;
	}
	
	//limit visible distance for big performance boost
	if(vec_dist(ent->x, player->x) > 2000)
		return;
		
	MARKER_initializeEntity(ent);
	
	MARKERdata * data = (void*)ent.markerData;
	
	str_cpy(data.contents, text);
	data.countdown = 1; // This mimics the previous behaviour, but stateful
}

void MARKER_update(ENTITY* ent)
{
	if(ent != NULL) {
		MARKER_setText(ent, (txtMarkers->pstring)[ent->type]);
	}
}
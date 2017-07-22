#include "camera.h"
#include "marker_cfg.h"

var MARKER__show = 0;

void MARKER_toggle()
{
	MARKER__show = 1 - MARKER__show;
}

void MARKER_attach(ENTITY* ent)
{
	if (ent != NULL)
	{
	//	set(ent, has_marker);
	}
	while(ent)
	{
		MARKER_update(ent);
		wait(1);
	}
}

void MARKER_detach(ENTITY* ent)
{
	if (ent != NULL)
	{
//		reset(ent, has_marker);
	}
}

void MARKER_update(ENTITY* ent)
{
	if (MARKER__show == 0)
		return;
	
	if (ent == NULL || player == NULL)
	{
		return;
	}
	if(vec_dist(ent->x, player->x) < 1000)
	{
		VIEW* view = get_camera();
		VECTOR vecTemp;
		vec_set(&vecTemp, ent->x);
		if ((vec_to_screen(vecTemp, get_camera()) != NULL) && (ent->type < txtMarkers->strings))
		{
			STRING* str = (txtMarkers->pstring)[ent->type];
			//draw_text(".jetpack\n 0xC0FFEE", vecTemp.x, vecTemp.y, vector(255,255,255));
			draw_text(str, vecTemp.x, vecTemp.y, vector(255,255,255));
		}
	}	
}
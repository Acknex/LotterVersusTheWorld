#include "camera.h"
#include "marker_cfg.h"

var MARKER__show = 0;
var MARKER__enable = 0;

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

void MARKER_update(ENTITY* ent)
{
	if (MARKER__show == 0 || MARKER__enable == 0)
		return;
	
	if (ent == NULL || player == NULL)
	{
		return;
	}

	//quick hack: only show when hovering with mouse
	//dumb solution but rewriting everything is more work
	//add delay counter to avoid flickering text
	if(mouse_ent == ent)
	{
		ent->markerCounter = 8;
	}
	else if (ent->markerCounter > 0)
	{
		ent->markerCounter = maxv(0, ent->markerCounter - time_step);
	}
	else
	{
		return;
	}
		
	if(vec_dist(ent->x, player->x) < 1500)
	{
		VIEW* view = get_camera();
		VECTOR vecTemp;
		vec_set(&vecTemp, ent->x);
		if ((vec_to_screen(vecTemp, get_camera()) != NULL) && (ent->type < txtMarkers->strings))
		{
			STRING* str = (txtMarkers->pstring)[ent->type];
			draw_text(str, vecTemp.x, vecTemp.y, vector(255,230,230));
		}
	}	
}
#ifndef FANCYHUD_H
#define FANCYHUD_H

function fancyhud_objects_event()
{
	if(my) {
		if(my.type == TypeMarker) {
			// draw all markers
			return 0;
		}
	}
	return 1;
}

MATERIAL * fancyhud_objects = 
{
	effect = "Marker.fx";
	event = fancyhud_objects_event;
	flags = ENABLE_RENDER;
}

VIEW * fancyhud_view = 
{
	material = fancyhud_objects;
	// layer = 3; // render before normal camera
	flags = SHOW | UNTOUCHABLE;
}

void fancyhud_init()
{
}

void fancyhud_update()
{
// Update the resized bmap!
	fancyhud_view.bmap = materials_FancyHUDImage;
	
	VIEW * cam = get_camera();
	
	fancyhud_view.arc    = cam.arc;
	fancyhud_view.fog_start = cam.fog_start;
	fancyhud_view.fog_end   = cam.fog_end;
	fancyhud_view.clip_far  = cam.clip_far;
	fancyhud_view.clip_near = cam.clip_near;
	
	vec_set(fancyhud_view.x, cam.x);
	vec_set(fancyhud_view.pan, cam.pan);
}

#endif
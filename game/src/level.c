#include "camera.h"

void LEVEL_start()
{
	level_load("prototype.mdl");
	me = ent_create("cube.mdl",nullvector, NULL);
	my->z+=50;
	//setup camera	
	focus_camera(me);
	show_camera();
	
	player = me;
}

void LEVEL_end()
{
	hide_camera();
}

void LEVEL_loop()
{
	while(1)
	{
		update_camera();
		wait(1);
	}
}
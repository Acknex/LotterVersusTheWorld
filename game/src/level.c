#include "camera.h"
#include "player.h"
#include "levelgen.h"

var LEVEL__running = 0;

STAGE* LEVEL__stage = NULL;

void LEVEL_start()
{
	LEVEL__running = 1;
	
	LEVEL__stage = stageCreate(32,32,137.1);
	stageFill(LEVEL__stage);
	stage_load(LEVEL__stage); // calls level_load!
	
	player_init();
	//setup camera	
	focus_camera(player);
	show_camera();
	
	pp_bloom(0.5, 0.5);
}

void LEVEL_end()
{
	// TODO: @superku: stageRemove or similar!
	stage_unload(); // destroy all meshes
	// stage_remove(LEVEL__stage);

	LEVEL__running = 0;
	
	wait(1);
	
	hide_camera();
}

void LEVEL_loop()
{
	while(LEVEL__running != 0)
	{
		player_move();
		update_camera();
		wait(1);
	}
}

void INIT_global()
{
	create_camera();	
}

void INIT_resize()
{
	//error("bling");
}

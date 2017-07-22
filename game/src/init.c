#include "camera.h"
#include "player.h"
#include "levelgen.h"

var INIT__levelRunning = 0;

STAGE* LEVEL__stage = NULL;

void INIT_levelStart()
{
	INIT__levelRunning = 1;
	
	//level_load("prototype_wmb.wmb");
	//level_load("prototype.mdl");
	//level_load("test_level_small.wmb");
	LEVEL__stage = stageCreate(32,32,137.1);
	stageFill(LEVEL__stage);
	stage_load(LEVEL__stage); // calls level_load!
	
	player_init();
	//setup camera	
	focus_camera(player);
	show_camera();
	
	pp_bloom(0.5, 0.5);
}

void INIT_levelEnd()
{
	// TODO: @superku: stageRemove or similar!
	// stage_remove(LEVEL__stage);

	INIT__levelRunning = 0;
	
	wait(1);
	
	hide_camera();
}

void INIT_levelLoop()
{
	while(INIT__levelRunning != 0)
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

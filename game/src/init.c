#include "camera.h"
#include "player.h"
#include "levelgen.h"
#include "font.h"
#include "sky.h"

#include "items.h" //temp
#include "turret.h" //temp

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
	
	ground_reflections();
	pp_bloom(0.3, 2.0);
	//skychange(); //because.
	
	//this is debug hack for items
	VECTOR* vecTemp;
	vecTemp = vector(175,175, 50);
	ent_create("jetpack_lotter.mdl", vecTemp, item_jetpack);

	vecTemp = vector(210,600, 50);
	ent_create(CUBE_MDL, vecTemp, enemy_turret);
}

void INIT_levelEnd()
{
	// TODO: @superku: stageRemove or similar!
	stage_unload(); // destroy all meshes
	// stage_remove(LEVEL__stage);

	INIT__levelRunning = 0;
	
	wait(1);
	
	hide_camera();
	//sky_active = 0;
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

void INIT_start()
{
	FONT_create();
	create_camera();	
}

void INIT_exit()
{
	//error("clean exit");
	FONT_remove();
}
	
void INIT_resize()
{
	pp_bloom_resize();
	FONT_scale();
}

#include "camera.h"
#include "player.h"
#include "levelgen.h"
#include "font.h"
#include "sky.h"
#include "music_player.h"

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
	
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0.1;
	
	player_init();
	//setup camera	
	focus_camera(player);
	show_camera();
	
	ground_reflections();
	pp_bloom(2.5);
	//skychange(); //because.
	

	startMusic("media\\in_game1.mp3", 4);

	//this is debug hack for items
	VECTOR* vecTemp;
	vecTemp = vector(175,175, 25);
	ent_create("jetpack_lotter.mdl", vecTemp, item_jetpack);
	vecTemp = vector(725,900, 50);
	ent_create(CUBE_MDL, vecTemp, enemy_turret);}

void INIT_levelEnd()
{
	// TODO: @superku: stageRemove or similar!
	stage_unload(); // destroy all meshes
	// stage_remove(LEVEL__stage);

	INIT__levelRunning = 0;
	
	stopMusic();
	
	wait(1);
	
	hide_camera();
	//sky_active = 0;
}

void INIT_levelLoop()
{
	while(INIT__levelRunning != 0)
	{
		updateMusic();
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

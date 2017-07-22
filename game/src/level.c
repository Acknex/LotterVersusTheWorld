#include "camera.h"
#include "player.h"

var LEVEL__running = 0;

void LEVEL_start()
{
	LEVEL__running = 1;
	level_load("prototype.mdl");
	//level_load("test_level_small.wmb");
	player_init();
	//setup camera	
	focus_camera(player);
	show_camera();
	
	pp_bloom(0.5, 0.5);
}

void LEVEL_end()
{
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
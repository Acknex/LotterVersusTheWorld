#include "camera.h"
#include "player.h"

void LEVEL_start()
{
	//level_load("prototype.mdl");
	level_load("test_level_small.wmb");
	player_init();
	//setup camera	
	focus_camera(player);
	show_camera();
	
	//player = me;
}

void LEVEL_end()
{
	hide_camera();
}

void LEVEL_loop()
{
	while(1)
	{
		player_move();
		update_camera();
		wait(1);
	}
}
#include "camera.h"
#include "sky.h"
#include "music_player.h"

void SPLASH__init()
{
	SPLASH__setupLevel();
	wait(1);
	
	startMusic("media\\main_menu.mp3", 4);
	
	//error("lol");
}

void SPLASH__animStart()
{
	
}

void SPLASH__animEnd()
{
	
}

void SPLASH__setupLevel()
{
	level_load(NULL);
		
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0.1;
}
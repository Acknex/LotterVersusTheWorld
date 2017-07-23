#include "camera.h"
#include "sky.h"
#include "music_player.h"


void SPLASH__init()
{
	SPLASH__setupLevel();
	wait(1);
	
	startMusic("media\\main_menu.mp3", 4, 0);
	
	SPLASH__animStart();
	
	//error("lol");
}

void SPLASH__animStart()
{
	while(SPLASH__cube->alpha < 100)
	{
		if(key_any == 1) return SPLASH__cancelAnim();
		SPLASH__cube->alpha += 1.75 * time_step;
		wait(1);
	}	
	
	if(key_any == 1) return SPLASH__cancelAnim();
	wait(-1);
	
	while(SPLASH__beam->z < 0)
	{
		if(key_any == 1) return SPLASH__cancelAnim();
		SPLASH__beam->z += 10 * time_step;
		wait(1);
	}
	
	if(key_any == 1) return SPLASH__cancelAnim();
	wait(-1);
	
	//var tempFactor = (1 - (SPLASH__logoPanel->alpha / 100));
	//var tempDist = screen_size.y - SPLASH__logoY;
	while(SPLASH__logoPanel->alpha < 100)
	{
		SPLASH__logoPanel->alpha += 1 * time_step;
		//tempFactor = (1 - (SPLASH__logoPanel->alpha / 100));
		//SPLASH__logoPanel->pos_y = screen_size.y - (tempDist * tempFactor);
		wait(1);
	}
	
	
	SPLASH__animEnd();	
}

int SPLASH__cancelAnim()
{
	SPLASH__animEnd();
	return 1;
}

void SPLASH__animEnd()
{
	SPLASH__cube->alpha = 100;
	SPLASH__beam->z = 0;	
}

void SPLASH__setupLevel()
{
	level_load(NULL);
		
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0.1;
	
	// Init A4 cube
	SPLASH__cube = ent_create("a4Cube.mdl", vector(750, -5, 120), NULL);
	SPLASH__cube->pan = -45;
	vec_scale(SPLASH__cube.scale_x, 1.25);
	set(SPLASH__cube, TRANSLUCENT);
	SPLASH__cube->alpha = 0;
	
	// Init Lotti
	SPLASH__lotti = ent_create("cbabe_male.mdl", vector(750, 0, 0), NULL);
	SPLASH__lotti->material = LotterMaterial;
	vec_scale(SPLASH__lotti.scale_x, 5);	
	SPLASH__lotti->pan = 180;
	ent_animate(SPLASH__lotti, "intro", 0, 0);
	//SPLASH__lotti->ambient = -100;
		
	
	// Init Tronbeam
	SPLASH__beam = ent_create("beam.png", vector(850, 0, -1000), NULL);
	SPLASH__beam->pan = 1;
	set(SPLASH__beam, LIGHT);
	set(SPLASH__beam, BRIGHT);
	SPLASH__beam->ambient = 100;
	vec_set(SPLASH__beam->scale_x, vector(1, 3, 0));
	vec_set(SPLASH__beam->blue, vector(255, 128,0));
	
	// Init logo
	// Shut up, I know I am calculating these twice
	var logoWidth = bmap_width(SPLASH__logoBmap);
	var logoHeight = bmap_height(SPLASH__logoBmap);
	SPLASH__logoPanel = pan_create(NULL, 1);
	pan_setwindow(SPLASH__logoPanel, 0, 0, 0, logoWidth, logoHeight, SPLASH__logoBmap, 0, 0);
	SPLASH__positionLogo();
	SPLASH__logoPanel->alpha = 0;
	set(SPLASH__logoPanel, TRANSLUCENT);
	set(SPLASH__logoPanel, SHOW);
	
	
	// Fix camera
	vec_set(cam->x, nullvector);
	cam->pan = 0;
	cam->tilt = 0; 
	
	wait(1);
	show_camera();
}

void SPLASH__positionLogo()
{
	if(SPLASH__inSplash && SPLASH__logoPanel != NULL && SPLASH__logoBmap != NULL)
	{
		var ratio = screen_size.x / 1920;
		var logoWidth = bmap_width(SPLASH__logoBmap);
		var logoHeight = bmap_height(SPLASH__logoBmap);
		SPLASH__logoY = (screen_size.y / 2) - ((logoHeight * ratio) / 2);
		SPLASH__logoPanel->scale_x = ratio;
		SPLASH__logoPanel->scale_y = ratio;
		SPLASH__logoPanel->pos_x = (screen_size.x / 2) - ((logoWidth * ratio) / 2); 
		SPLASH__logoPanel->pos_y = SPLASH__logoY; 
		//SPLASH__logoPanel->pos_y = screen_size.y; 
	}
}
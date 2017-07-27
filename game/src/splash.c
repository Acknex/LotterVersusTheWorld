#include <particles.c>

#include "camera.h"
#include "sky.h"
#include "music_player.h"
#include "mouse.h"
#include "stats.h"


void SPLASH__init()
{
	create_camera();
	show_camera(); // Don't forget: Normal camera is still visible ^^
	pp_bloom_start(2.0);
	wait(10); // this is required: wait until fully init
	SPLASH__setupLevel();
	// wait(1);
	
	startMusic("media\\main_menu.mp3", 4, 0);
	
	SPLASH__animStart();
}

int SPLASH__animStart()
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
		SPLASH__beam->z += 15 * time_step;
		wait(1);
	}
	
	if(key_any == 1) return SPLASH__cancelAnim();
	wait(-1);
	
	while(SPLASH__lotti->alpha < 100)
	{
		if(key_any == 1) return SPLASH__cancelAnim();
		SPLASH__lotti->alpha += 2.5 * time_step;
		wait(1);
	}	
	
	if(key_any == 1) return SPLASH__cancelAnim();
	wait(-1);
	
	var tempFactor = (1 - (SPLASH__logoPanel->alpha / 50));
	while(SPLASH__logoPanel->alpha < 50)
	{
		var tempDist = screen_size.y - SPLASH__logoY;
		if(key_any == 1) return SPLASH__cancelAnim();
		SPLASH__logoPanel->alpha += 1 * time_step;
		tempFactor = (SPLASH__logoPanel->alpha / 50);
		SPLASH__logoPanel->pos_y = screen_size.y - (tempDist * tempFactor);
		wait(1);
	}
	
	
	SPLASH__animEnd();
	return 0;	
}

int SPLASH__cancelAnim()
{
	SPLASH__animEnd();
	return 42;
}

void SPLASH__animEnd()
{
	SPLASH__cube->alpha = 100;
	SPLASH__lotti->alpha = 100;
	SPLASH__beam->z = 0;	
	SPLASH__logoPanel->pos_y = SPLASH__logoY;
	SPLASH__logoPanel->alpha = 50;
	
	while(key_any)
	{
		wait(1);
	}
	
	mouse_init_game();	
	
	while(SPLASH__menuPanel->alpha < 50 && SPLASH__inSplash == 1)
	{
		if(key_any == 1 || SPLASH__inSplash == 0) break;
		SPLASH__menuPanel->alpha += 2 * time_step;
		wait(1);
	}
	
	if(SPLASH__inSplash == 1)
	{
		SPLASH__menuPanel->alpha = 50;
	}	
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
	SPLASH__lotti->material = LotterSplashMaterial;
	vec_scale(SPLASH__lotti.scale_x, 5);	
	SPLASH__lotti->pan = 180;
	ent_animate(SPLASH__lotti, "intro", 0, 0);
	set(SPLASH__lotti, TRANSLUCENT);
	SPLASH__lotti->alpha = 0;
	
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
	SPLASH__logoPanel->alpha = 0;
	set(SPLASH__logoPanel, TRANSLUCENT);
	set(SPLASH__logoPanel, SHOW);
	
	// Init Menu
	SPLASH__menuPanel = pan_create(NULL, 2);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 0, SPLASH__menuStartOnBmap, SPLASH__menuStartOffBmap, SPLASH__menuStartOnBmap, SPLASH__menuStartOffBmap, SPLASH__startGame, NULL, NULL);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 32, SPLASH__menuCreditsOnBmap, SPLASH__menuCreditsOffBmap, SPLASH__menuCreditsOnBmap, SPLASH__menuCreditsOffBmap, SPLASH__startCredits, NULL, NULL);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 64, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__exitGame, NULL, NULL);
	SPLASH__menuPanel->alpha = 0;
	set(SPLASH__menuPanel, TRANSLUCENT);
	set(SPLASH__menuPanel, SHOW);	
	
	SPLASH__reposition();
	
	// Fix camera
	vec_set(cam->x, nullvector);
	cam->pan = 0;
	cam->tilt = 0; 
	
	// Intro stuff last
	SPLASH__smokeEmitter = ent_create("a4Cube.mdl", vector(-750, 0, 0), SPLASH__act_smokeGen);
}


// Someone smarter than me please put this into a logical place inside the global resize function
void SPLASH__reposition()
{
	if(SPLASH__inSplash)
	{
		if(SPLASH__logoPanel != NULL && SPLASH__logoBmap != NULL && SPLASH__menuPanel != NULL)
		{
			var ratio = screen_size.x / 1920;
			var logoWidth = bmap_width(SPLASH__logoBmap);
			var logoHeight = bmap_height(SPLASH__logoBmap);
			SPLASH__logoY = (screen_size.y / 2) - ((logoHeight * ratio) / 2);
			SPLASH__logoPanel->scale_x = ratio;
			SPLASH__logoPanel->scale_y = ratio;
			SPLASH__logoPanel->pos_x = (screen_size.x / 2) - ((logoWidth * ratio) / 2); 
			//while animated (alpha <50) logo corrects position by itself
			if (SPLASH__logoPanel->alpha >= 50)
				SPLASH__logoPanel->pos_y = SPLASH__logoY; 
			//SPLASH__logoPanel->pos_y = screen_size.y; 
			
			SPLASH__menuPanel->pos_x = (screen_size.x / 2) - (64);
			SPLASH__menuPanel->pos_y = SPLASH__logoY + logoHeight;
			
		}
	}
}

void SPLASH__initMenu()
{
	// Guess I didn't need this?
}

void SPLASH__startGame()
{
	SPLASH__housekeeping();	
	
	wait(1);
	
	// reset stats when the game starts
	stats_reset();
	
	wait(1);
	INIT_levelStart();
	INIT_levelLoop();
}

void SPLASH__startCredits()
{
	// Hier den credits start code einfügen. 
}

void SPLASH__introStart()
{
}

void SPLASH__introEnd()
{
}

void SPLASH__introCancel()
{
}

function SPLASH__act_smoke(ENTITY* p)
{
	VECTOR vTemp;
	vec_randomize(vTemp, 3);
	vec_add(p._VEL_X, vTemp);
	 
	set(p, _MOVE | TRANSLUCENT);
	p.alpha = 100;
	//p.bmap = SPLASH__smokeSprite;
	//part.lifespan = 100;
	//part.size = 200;
	//part.event = SPLASH__act_smoke_event;
	//part.gravity = 0.01;
	vec_set(p.blue, vector(60, 60, 60));
	p._SIZE = 20;
	p._GRAVITY = 0.01;
	p._FADE = 0.5;  // fade factor
	p._LIFESPAN = 1000;
	p.event = p_fade_sprite;	
}

action SPLASH__act_smokeGen()
{
	VECTOR emitterTemp;
	set(me, INVISIBLE);
	while(me)
	{
		vec_randomize(emitterTemp, my->x);
		effect_sprite("smoke.tga", SPLASH__act_smoke, maxv(1,2*time_step), my->x, vector(0, 0, 1));
		wait(1);
	}
}

void SPLASH__exitGame()
{
	sys_exit("");
}

void SPLASH__housekeeping()
{
	SPLASH__inSplash = 0;
	remove_camera();
	ptr_remove(SPLASH__logoPanel);
	ptr_remove(SPLASH__menuPanel);
	ptr_remove(SPLASH__logoBmap);
	ptr_remove(SPLASH__menuStartOffBmap);
	ptr_remove(SPLASH__menuStartOnBmap);
	ptr_remove(SPLASH__menuCreditsOffBmap);
	ptr_remove(SPLASH__menuCreditsOnBmap);
	ptr_remove(SPLASH__menuExitOffBmap);
	ptr_remove(SPLASH__menuExitOnBmap);
	ent_remove(SPLASH__cube);
	ent_remove(SPLASH__beam);
	ent_remove(SPLASH__lotti);
	
	SPLASH__logoPanel = NULL;
	SPLASH__menuPanel = NULL;	
	SPLASH__logoBmap = NULL;
	SPLASH__menuStartOffBmap = NULL;
	SPLASH__menuStartOnBmap = NULL;
	SPLASH__menuCreditsOffBmap = NULL;
	SPLASH__menuCreditsOnBmap = NULL;
	SPLASH__menuExitOffBmap = NULL;
	SPLASH__menuExitOnBmap = NULL;
	SPLASH__cube = NULL;
	SPLASH__beam = NULL;
	SPLASH__lotti = NULL;
}
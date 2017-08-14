#include <particles.c>

#include "camera.h"
#include "sky.h"
#include "music_player.h"
#include "mouse.h"
#include "stats.h"
#include "credits.h"

#define INTRO_NODE_FLY_TIME 4

STRING * SPLASH__highscoreText = "#128";
FONT * SPLASH__highscoreFont = "Arial#24b";

TEXT * SPLASH__highscoreObject = 
{
	string (SPLASH__highscoreText);
	font = SPLASH__highscoreFont;
	flags = CENTER_X | OUTLINE | TRANSLUCENT;
}


typedef struct IntroNode
{
	var time;
	var length;
	var out;
	STRING * img;
} IntroNode;

int INTRO__nodesDone = 0;

function introNodeFunc() {
	set(my, TRANSLUCENT);
	my.alpha = 0;
	var t = 0;
	var x = random(70)+50;
	var y = random(70)+50;
	if(random(2) > 1) {
		x*=-1;
	}
	
	if(random(2) > 1) {
		y*= -1;
	}
	
	while(t < INTRO_NODE_FLY_TIME) {
		t = minv(t+time_step/16, INTRO_NODE_FLY_TIME);
		my.pan = -x*(1-t/INTRO_NODE_FLY_TIME)*0.4;
		my.tilt = -y*(1-t/INTRO_NODE_FLY_TIME)*0.4;
		vec_set(my.x, vector(-1200 + 900*t/INTRO_NODE_FLY_TIME, x*(1-t/INTRO_NODE_FLY_TIME), y*(1-t/INTRO_NODE_FLY_TIME)));
		my.alpha = 100*t/INTRO_NODE_FLY_TIME;
		wait(1);
	}
	t = 0;
	while(t < my.skill34) {
		my.alpha = 100;
		t = minv(t + time_step/16, my.skill34);
		wait(1);
	}
	
	t = 0;
	while(t < my.skill35) {
		t = minv(t + time_step/16, my.skill35);
		vec_set(my.x, vector(-300 + 50*t/my.skill35, 0, 0));
		my.alpha = 100 - maxv(0,minv(1,(t-my.skill35+0.3)/0.3))*100;
		wait(1);
	}
	ptr_remove(my);
	INTRO__nodesDone++;
}

void SPLASH__quitGame()
{
	//do not add anything here - use INIT_exit in init.c!
	sys_exit("");
}

void SPLASH__init()
{
	// Kill the game when pressing escape in main menu
	on_esc = SPLASH__quitGame;
	
	SPLASH__inSplash = 1;
	create_camera();
	show_camera(); // Don't forget: Normal camera is still visible ^^
	pp_bloom_start(2.0);
	wait(10); // this is required: wait until fully init
	SPLASH__setupLevel();
	// wait(1);
	
	while(key_any)
	{
		wait(1);
	}
	startMusic("media\\main_menu.mp3", 4, 0);
	
	SPLASH__animStart();
	
	proc_mode = PROC_LATE;
	while(SPLASH__inSplash)
	{
		SPLASH__highscoreObject.alpha = clamp(
			2 * SPLASH__menuPanel.alpha,
			0,
			100);
		wait(1);
	}
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
	
	str_printf(
		SPLASH__highscoreText,
		"Highscore: %d",
		(int)stats_highscore.score);
	set(SPLASH__highscoreObject, SHOW);
	SPLASH__highscoreObject.alpha = 0;
	
	// Init Menu
	SPLASH__menuPanel = pan_create(NULL, 2);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 0, SPLASH__menuStartOnBmap, SPLASH__menuStartOffBmap, SPLASH__menuStartOnBmap, SPLASH__menuStartOffBmap, SPLASH__introStart, NULL, NULL);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 32, SPLASH__menuCreditsOnBmap, SPLASH__menuCreditsOffBmap, SPLASH__menuCreditsOnBmap, SPLASH__menuCreditsOffBmap, SPLASH__startCredits, NULL, NULL);
	pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 64, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__exitGame, NULL, NULL);
	// pan_setbutton(SPLASH__menuPanel, 0, 0, 0, 128, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__menuExitOnBmap, SPLASH__menuExitOffBmap, SPLASH__introStart, NULL, NULL);
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
			
			SPLASH__highscoreObject.pos_x = (screen_size.x / 2);
			SPLASH__highscoreObject.pos_y = SPLASH__menuPanel->pos_y + 96;
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
	SPLASH__housekeeping();
	
	wait(1);
	
	on_esc = credits_cancel;
	credits_start();
}

var _smoothstep(var x, var edge0, var edge1)
{
	var t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

void SPLASH__introStart()
{
	// prevent double-start
	if(SPLASH__inIntro) {
		return;
	}
	on_esc = SPLASH__introCancel;
	on_anykey = SPLASH__introCancel;
	startMusic(NULL, 4, 0); // fade out music
	
	SPLASH__inIntro = 1;
	
	var panSpeed = 0.0;
	
	while(cam->pan < 180 && SPLASH__inIntro)
	{
		panSpeed = minv(3, panSpeed + 0.05 * time_step);
		cam.pan += panSpeed * time_step;
		
		SPLASH__logoPanel->alpha = 100 - ((cam->pan / 180) * 100);
		
		SPLASH__menuPanel->alpha = maxv(0, SPLASH__menuPanel->alpha - 25 * time_step);
		if(SPLASH__menuPanel->alpha <= 0) {
			reset(SPLASH__menuPanel, SHOW);
		}
		
		// TODO: remove this line
		//cam->pan = 180;
		wait(1);
	}
	
	if(SPLASH__inIntro != 1) 
	{
		SPLASH__introEnd();
		return;
	}
	
	cam->pan = 180; // just to make sure we are absolutely bang on
	
	startMusic("media\\intro.mp3", 0.01, 0);
	wait(1);
	
	var countNodes = 7;
	IntroNode nodes[7];
	nodes[0].time = 2.3; nodes[0].img = "intro04.tga"; nodes[0].length = 13; nodes[0].out = 3;
	nodes[1].time = 22; nodes[1].img = "intro03.tga"; nodes[1].length = 7; nodes[1].out = 5.5;
	nodes[2].time = 36.0; nodes[2].img = "intro07.tga"; nodes[2].length = 4; nodes[2].out = 2;
	nodes[3].time = 44.0; nodes[3].img = "intro06.tga"; nodes[3].length = 2; nodes[3].out = 3;
	nodes[4].time = 50.6; nodes[4].img = "intro01.tga"; nodes[4].length = 10; nodes[4].out = 5;
	nodes[5].time = 67.0; nodes[5].img = "intro05.tga"; nodes[5].length = 10; nodes[5].out = 8;
	nodes[6].time = 100.0; nodes[6].img = "intro02.tga"; nodes[6].length = 1; nodes[6].out = 3;
	// Setup scene 1
	//SPLASH__introEnt1 = ent_create("intro02.tga", vector(-300, 0, 0), NULL);

	//var s = 0.1;
	//vec_set(SPLASH__introEnt1.scale_x, vector(s,s,s));
	
	INTRO__nodesDone = 0;
	var currentNode = 0;
	while(INTRO__nodesDone < countNodes && SPLASH__inIntro)
	{
		if(currentNode < countNodes)
		{
			if(getMusicPosition() > nodes[currentNode].time-INTRO_NODE_FLY_TIME) {
				ENTITY* a = ent_create(nodes[currentNode].img, vector(-900, 0, 0), introNodeFunc);
				a.skill34 = nodes[currentNode].length;
				a.skill35 = nodes[currentNode].out;
				var s = 0.1;
				vec_set(a.scale_x, vector(s,s,s));
				currentNode += 1;
			}
		}
		wait(1);
	}
	
	SPLASH__introEnd();
	/*
	SPLASH__introEnt1 = ent_create("tree02.mdl", vector(-20, -4, 0), NULL);
	SPLASH__introEnt2 = ent_create("cbabe_male.mdl", vector(-300, 0, 0), NULL);
	
	set(SPLASH__introEnt1, TRANSLUCENT);
	SPLASH__introEnt1->alpha = 0;
//	SPLASH__introEnt1->material = LotterMaterial;
	set(SPLASH__introEnt2, TRANSLUCENT);
	SPLASH__introEnt2->alpha = 0;
//	SPLASH__introEnt2->material = LotterMaterial;
	ent_animate(SPLASH__introEnt2, "speak", 0, 0);
	wait(1);
	
	while(SPLASH__introEnt1->alpha < 100)
	{
		SPLASH__introEnt1->alpha += 2 * time_step;
		SPLASH__introEnt2->alpha += 2 * time_step;
		wait(1);
	}	
	*/
	
}

void SPLASH__introEnd()
{
	on_anykey = NULL;
	on_esc    = NULL;
	SPLASH__startGame();
}

void SPLASH__introCancel()
{
	SPLASH__inIntro = 0;
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
	vec_set(p.blue, vector(random(30) + 50 + SPLASH__blueBias, random(30) + 50 + SPLASH__greenBias, random(30) + 50 + SPLASH__redBias));
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
		if(SPLASH__inIntro == 1)
		{
			vec_randomize(emitterTemp, my->x);
			effect_sprite("smokex.tga", SPLASH__act_smoke, maxv(1, 2*time_step), my->x, vector(0, 0, 1));
		}
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
	reset(SPLASH__highscoreObject, SHOW);
	//ptr_remove(SPLASH__logoBmap);
	//ptr_remove(SPLASH__menuStartOffBmap);
	//ptr_remove(SPLASH__menuStartOnBmap);
	//ptr_remove(SPLASH__menuCreditsOffBmap);
	//ptr_remove(SPLASH__menuCreditsOnBmap);
	//ptr_remove(SPLASH__menuExitOffBmap);
	//ptr_remove(SPLASH__menuExitOnBmap);
	ent_remove(SPLASH__cube);
	ent_remove(SPLASH__beam);
	ent_remove(SPLASH__lotti);
	
	SPLASH__logoPanel = NULL;
	SPLASH__menuPanel = NULL;	
	//SPLASH__logoBmap = NULL;
	//SPLASH__menuStartOffBmap = NULL;
	//SPLASH__menuStartOnBmap = NULL;
	//SPLASH__menuCreditsOffBmap = NULL;
	//SPLASH__menuCreditsOnBmap = NULL;
	//SPLASH__menuExitOffBmap = NULL;
	//SPLASH__menuExitOnBmap = NULL;
	SPLASH__cube = NULL;
	SPLASH__beam = NULL;
	SPLASH__lotti = NULL;
}
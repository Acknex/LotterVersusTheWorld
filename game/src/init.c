#include "camera.h"
#include "player.h"
#include "levelgen.h"
#include "font.h"
#include "sky.h"
#include "music_player.h"
#include "hud.h"
#include "quest.h"
#include "cheats.h"
#include "stats.h"
#include "fancyhud.h"
#include "splash.h"

var INIT__levelRunning = 0;
var INIT__currentHardness = 0;

function INIT_levelRestartCheat()
{
	if(cheats_enabled) INIT_levelEnd(0);
}

//
// NO wait(1) IN INIT_levelStart:
// Will crash horribly because game loop is started
//
void INIT_levelStart()
{
	on_r = INIT_levelRestartCheat;
	
	INIT__levelRunning = 1;

	random_seed(0);
	QUEST_init();	
	//wer auch immer hier ein INIT__currentHardness++ versteckt hat: Schelle links, Schelle rechts!
	LEVEL__stage = stageCreate(16+INIT__currentHardness,16+INIT__currentHardness,0,INIT__currentHardness++); // 8172.607
	stageFill(LEVEL__stage);
	stageConnect(LEVEL__stage);
	stageAddExitAndEntrance(LEVEL__stage);
	stageCreateEnemyData(LEVEL__stage);

	stage_load(LEVEL__stage); // calls level_load!

	VECTOR* vecTemp;
	vecTemp = stageGetQuestPosition(LEVEL__stage, NULL, 0.85, 0.1);
	ent_create("warlock.mdl", vecTemp, questmaster);
	vecTemp = stageGetQuestPosition(LEVEL__stage, NULL, 0.25, 0.1);
	ent_create(CUBE_MDL, vecTemp, questitem);
	
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0.1;
	
	player_init();
	
	//setup camera	
	create_camera();
	focus_camera(player);
	show_camera();
	
	// Use dummy entity for "level outliving"
	ent_create(NULL, NULL, ground_reflections);
	pp_bloom_start(2.5);
	
	mouse_init_game();
	hud_ingame_init();
	hud_ingame_show();
	//skychange(); //because.
	

	startMusic("media\\in_game1.mp3", 4, 1);

	enemy_spawn_hex();
	MARKER_attach();
	
	wait(-0.5);
	
	show_dialog(str_printf(
		NULL,
		"Welcome to Stage 0x%02X.\nEnjoy your stay!",
		(int)INIT__currentHardness));
}

void INIT_levelEnd(var isGameOver)
{
	me = NULL; //decouple from any calling entity
	INIT__levelRunning = 0;
	
	MARKER_detach();
	
	proc_kill2(MARKER_sprite, NULL); //is this still needed?
	
	wait(1);
	
	pp_bloom_stop();
	
	level_load(NULL);
	
	// stage_unload(); // destroy all meshes
	stageDestroy(LEVEL__stage);
	LEVEL__stage = NULL;
	
	wait(1);
	
	if(stats_finalize())
	{
		// TODO: Display highscore here
		error("You got a new highscore.\nAlso: Replace this error with something useful.");
	}
	
	if(isGameOver)
	{
		INIT__currentHardness = 0; //reset difficulty level after death
		stats_reset();
	}
	
	remove_camera();
	
	wait(-1);
	
	//sky_active = 0;
	INIT_levelStart();
	INIT_levelLoop();
}

void INIT_levelLoop()
{
	while(INIT__levelRunning != 0)
	{
		//updateMusic();
		if(player) if(player->health <= 0) {
			var fade = 0;
			show_death_screen(0);
			while(1)
			{
				show_death_screen(fade);
				fade = minv(fade+2*time_step, 100);
				if(fade > 80 && key_any)
					break;
				wait(1);
			}
			hide_death_screen();
			INIT_levelEnd(1);
			return;
		}
		player_move();
		update_camera();
		hud_ingame_update();
		fancyhud_update();
		
		if(cheats_enabled && LEVEL__stage && key_m) stageDraw(LEVEL__stage, 0, screen_size.y-LEVEL__stage->size[1]*12, 12);
		wait(1);
	}
}

void INIT_globalLoop()
{
	while(1)
	{
		updateMusic();
		wait(1);
	}
}

void INIT_start()
{
	stats_init();
	FONT_create();
	fancyhud_init();
	MARKER_init();
	MARKER_toggle();
	//create_camera();
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
	hud_ingame_align();
	SPLASH__reposition();
}

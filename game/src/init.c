#include "camera.h"
#include "player.h"
#include "levelgen.h"
#include "font.h"
#include "sky.h"
#include "music_player.h"
#include "hud.h"
#include "quest.h"

#include "items.h" //temp
#include "turret.h" //temp

var INIT__levelRunning = 0;
var INIT__currentHardness = 0;

void INIT_levelEnd();

//
// NO wait(1) IN INIT_levelStart:
// Will crash horribly because game loop is started
//
void INIT_levelStart()
{
	on_r = INIT_levelEnd;

	INIT__levelRunning = 1;

	LEVEL__stage = stageCreate(16+INIT__currentHardness,16+INIT__currentHardness,0,INIT__currentHardness++); // 8172.607
	stageFill(LEVEL__stage);
	stageConnect(LEVEL__stage);
	stageAddExitAndEntrance(LEVEL__stage);
	stageCreateEnemyData(LEVEL__stage);

	stage_load(LEVEL__stage); // calls level_load!

	QUEST_init();	
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
	pp_bloom(2.5);
	
	mouse_init_game();
	hud_ingame_init();
	hud_ingame_show();
	//skychange(); //because.
	

	startMusic("media\\in_game1.mp3", 4, 1);

	//this is debug hack for items
//	VECTOR* vecTemp;
//	vecTemp = vector(175,175, 25);
//	ent_create("jetpack_lotter.mdl", vecTemp, item_jetpack);
//	vecTemp = vector(725,900, 50);
//	you = ent_create("tronding1.mdl", vecTemp, enemy_turret);
//	you.material = ObjectMaterial;

	enemy_spawn_hex();
	MARKER_attach();
}

void INIT_levelEnd()
{
	me = NULL; //decouple from any calling entity
	INIT__levelRunning = 0;
	
	MARKER_detach();
	stopMusic();
	
	wait(1);
	
	diag_status();
	
	level_load(NULL);
	
	// stage_unload(); // destroy all meshes
	stageDestroy(LEVEL__stage);
	LEVEL__stage = NULL;
	
	wait(1);
	
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
		player_move();
		update_camera();
		hud_ingame_update();
		
		if(LEVEL__stage && key_m) stageDraw(LEVEL__stage, 0, screen_size.y-LEVEL__stage->size[1]*12, 12);
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
	FONT_create();
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
}

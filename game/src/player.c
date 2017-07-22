#include "camera.h"
#include "datatypes.h"
#include "levelgen.h"
#include "entity_defs.h"

STAGE* LEVEL__stage = NULL;

var anim_percentage = 0;
var dist_ahead = 0;
var dist_strafe = 0;
VECTOR *mousepos, playerpos;
ANGLE dir;

void player_move() {
	
	if (mouse_mode > 0)	
	{ 
		mouse_pos.x = mouse_cursor.x;    
		mouse_pos.y = mouse_cursor.y;
	}
	
	VIEW* view = get_camera();
	vec_set(playerpos, player->x);
	
	VECTOR vTarget;
	VECTOR to;
	ANGLE tAngle;
	vec_set(to,mouse_dir3d);
	vec_scale(to,5000); // set a range
	vec_add(to, mouse_pos3d);
	you = player;
	c_trace(mouse_pos3d, to, IGNORE_YOU | IGNORE_FLAG2);
	
	//draw_point3d(to, COLOR_WHITE, 100, 20);
	
	vec_set(vTarget, hit.x);
	vec_sub(vTarget, player.x);
	vec_to_angle(tAngle, vTarget);
	player.pan = tAngle.pan;
	
	dist_ahead = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (clamp(key_w + key_cuu, 0, 1) - clamp(key_s + key_cud, 0, 1));
	dist_strafe = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (clamp(key_a + key_cul, 0, 1) - clamp(key_d + key_cur, 0, 1));
	if (dist_ahead != 0 && dist_strafe != 0)
	{
		dist_ahead /= sqrt(2);
		dist_strafe /= sqrt(2);
	}
	VECTOR* vecDir = vector(dist_ahead, dist_strafe, 0);
	vec_scale(vecDir, time_step);
	
	//transform with camera angle
	vec_rotate(vecDir, vector(view->pan, 0, 0));
	
	if (move_style == 0) {
		c_move(player, nullvector, vecDir, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
	} else {
		c_move(player, vector(dist_ahead * time_step, 0, 0), nullvector, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
	}
	
	//#ifdef DEBUG
		DEBUG_VAR(dist_ahead, 260);
		DEBUG_VAR(dist_strafe, 280);
	//#endif
	
	// animation
	if (dist_ahead != 0 || dist_strafe != 0) {
		if (key_shiftl) {
			// run
			anim_percentage += 0.6*maxv(abs(dist_ahead), abs(dist_strafe)) * time_step;
			ent_animate(player,"walk", anim_percentage, ANM_CYCLE);
			} else {
			//walk
			anim_percentage += 0.6*maxv(abs(dist_ahead), abs(dist_strafe)) * time_step;
			ent_animate(player,"walk",anim_percentage,ANM_CYCLE);
		}	
		ent_blendframe(player, player, "strafe", 0, 25);
		} else {
		anim_percentage += 5*time_step; 
		ent_animate(player,"stand",anim_percentage,ANM_CYCLE);
	}

	if(LEVEL__stage) 
	{
		static int playerX = 0;
		static int playerY = 0;
		int x,y;
		x = floor((player.x-100)/200);
		y = floor((player.y-100)/200);
		if(playerX != x || playerY != y)
		{
			playerX = x;
			playerY = y;		
			stageDoFlood(LEVEL__stage, playerX, playerY, FLOOD_PLAYER, 12, 0);
		}
	}
}


VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py);

void player_init() {
	player = ent_create("cbabe_male.mdl", stageGetEntrancePos(LEVEL__stage, NULL, NULL, NULL), NULL);
	player->material = LotterMaterial;
	
	// Adapt scale
	vec_scale(player.scale_x, 1.2);
	
	// Adapt bounding box
	c_setminmax(player);
	vec_mul(player.min_x, vector(0.3, 0.5, 1.0));
	vec_mul(player.max_x, vector(0.5, 0.5, 1.0));
	
	move_friction = 0.1;
	
	mouse_mode = 4;
	player.z -= player.min_z;
	player->trigger_range = 20;
	
	player.damage = 1;
	
	player.emask |= EVENT_SHOOT;
	player.event = player_event;
}

void player_event() {
	switch(event_type) {
		case EVENT_SHOOT:
		my.health -=your.damage;
		if (my.health <= 0) {
			printf("You are DEAD!");
		}
		break;
	}
}

#include "camera.h"
#include "datatypes.h"
#include "levelgen.h"

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
	if (vec_to_screen(playerpos, view) != NULL)
	{
	
		vec_sub(playerpos, mouse_pos);
		vec_to_angle(dir, playerpos);	
		
	}
	player.pan = 180-dir.pan - view->pan;
	
	draw_textmode("Arial", 1, 20, 100);
	
	#ifdef DEBUG
	DEBUG_VAR(player.min_x, 100);
	DEBUG_VAR(player.min_y, 120);
	DEBUG_VAR(player.min_z, 140);
	DEBUG_VAR(player.max_x, 160);
	DEBUG_VAR(player.max_y, 180);
	DEBUG_VAR(player.max_z, 200);
	#endif
	
	
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
	c_move(player, nullvector, vecDir, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
	
	//#ifdef DEBUG
	DEBUG_VAR(dist_ahead, 260);
	DEBUG_VAR(dist_strafe, 280);
	//#endif
	
	// animation
	if (dist_ahead != 0 || dist_strafe != 0) {
		if (key_shiftl) {
			anim_percentage += 0.15*maxv(abs(dist_ahead), abs(dist_strafe));
			ent_animate(player,"run", anim_percentage, ANM_CYCLE);
		} else {
			anim_percentage += 0.2*maxv(abs(dist_ahead), abs(dist_strafe));
			ent_animate(player,"walk",anim_percentage,ANM_CYCLE);
		}		
	} else {
		anim_percentage += 5*time_step; 
		ent_animate(player,"idle",anim_percentage,ANM_CYCLE);
	}
}


VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py);

void player_init() {
	player = ent_create("cbabe.MDL", stageGetEntrancePos(LEVEL__stage, NULL, NULL, NULL), NULL);
	
	// Adapt scale
	vec_scale(player.scale_x, 2);
	
	// Adapt bounding box
	c_setminmax(player);
	vec_mul(player.min_x, vector(0.3, 0.5, 1.0));
	vec_mul(player.max_x, vector(0.5, 0.5, 1.0));
	
	move_friction = 0.1;
	
	mouse_mode = 4;
	player.z -= player.min_z;
	player->trigger_range = 20;
}

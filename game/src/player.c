#include "camera.h"

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
	DEBUG_VAR(player.pan, 100);
	
	
//	dist_ahead = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_w - key_s) * time_step;
//	dist_strafe = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_a - key_d) * time_step;
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
}

void player_init() {
	player = ent_create(CUBE_MDL, vector(0,0,0), NULL);
	vec_scale(player.scale_x, 3);
	c_setminmax(player);
	mouse_mode = 4;
	player.z -= player.min_z;
	player->trigger_range = 20;
}

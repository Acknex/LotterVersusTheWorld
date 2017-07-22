#include "camera.h"

var anim_percentage = 0;
var dist_ahead = 0;
var dist_strafe = 0;
VECTOR *mousepos, *playerpos, *dist;
ANGLE dir;

void player_move() {
	
 	if (mouse_mode > 0)	
  	{ 
    	mouse_pos.x = mouse_cursor.x;    
    	mouse_pos.y = mouse_cursor.y;
  	}
	
	//fix me with vec_for_screen
	mousepos = vector(mouse_pos.x, mouse_pos.y, 0);
	playerpos = vector(screen_size.x * 0.5, screen_size.y * 0.5, 0);
	dist = vec_sub(playerpos, mousepos);
	vec_to_angle(dir, dist);	
	player.pan = -dir.pan;
	
	DEBUG_VAR(player.pan, 100);
	
	
//	dist_ahead = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_w - key_s) * time_step;
//	dist_strafe = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_a - key_d) * time_step;
	dist_ahead = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_w - key_s);
	dist_strafe = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (key_a - key_d);
	if (dist_ahead != 0 && dist_strafe != 0)
	{
		dist_ahead /= sqrt(2);
		dist_strafe /= sqrt(2);
	}
	VECTOR* vecDir = vector(dist_ahead, dist_strafe, 0);
	vec_scale(vecDir, time_step);
	//transform with camera angle
	VIEW* view = get_camera();
	vec_rotate(vecDir, vector(view->pan, 0, 0));
	c_move(player, nullvector, vecDir, IGNORE_PASSABLE | GLIDE);
}

void player_init() {
	player = ent_create(CUBE_MDL, vector(0,0,0), NULL);
	vec_scale(player.scale_x, 3);
	c_setminmax(player);
	mouse_mode = 4;
	player.z -= player.min_z;
}

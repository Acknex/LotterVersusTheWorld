void player_move() {
	if (key_w) {
		player.x +=5 * time_step;
	}
	if (key_s) {
		player.x +=5 * time_step;
	}
	if (key_a) {
		player.y +=5 * time_step;
	}
	if (key_d) {
		player.y -=5 * time_step;
	}
	
 	if (mouse_mode > 0)	
  	{ 
    	mouse_pos.x = mouse_cursor.x;    
    	mouse_pos.y = mouse_cursor.y;
  	}
	
	VECTOR* mousepos = vector(mouse_pos.x, mouse_pos.y, 0);
	VECTOR* playerpos = vector(screen_size.x * 0.5, screen_size.y * 0.5, 0);
	VECTOR* dist = vec_sub(playerpos, mousepos);
	ANGLE dir;
	vec_to_angle(dir, dist);	
	player.pan = -dir.pan;
	
	DEBUG_VAR(player.pan, 10);
}

void player_init() {
	player = ent_create(CUBE_MDL, vector(0,0,0), NULL);
	mouse_mode = 4;
}
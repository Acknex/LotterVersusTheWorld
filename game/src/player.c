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
}

void player_init() {
	player = ent_create(SPHERE_MDL, vector(0,0,0), NULL);
}
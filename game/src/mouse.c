void mouse_init_game() {
	mouse_mode = 4;
	mouse_map = bmapCrosshair;
	mouse_spot.x = bmap_width(bmapCrosshair) / 2;
	mouse_spot.y = bmap_height(bmapCrosshair) / 2;
}
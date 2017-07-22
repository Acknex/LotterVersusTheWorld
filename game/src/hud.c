void hud_ingame_init() {
	if (!panEmoHealth) {
		panEmoHealth = pan_create("", 1);
		pan_setwindow(panEmoHealth, 0, 0, 0, bmap_width(bmapEmoHealth) / 4, bmap_height(bmapEmoHealth), bmapEmoHealth, vEmoHealthX, 0);
	}
	hud_ingame_align();
}

void hud_ingame_align() {
	if (panEmoHealth) {
		panEmoHealth.pos_x = 10;
		panEmoHealth.pos_y = screen_size.y - bmap_height(bmapEmoHealth);
	}
}

void hud_ingame_update() {
	if (!player) return;
	
	if (player.health > 75) {
		vEmoHealthX = 69 * 3;
	}
	if (player.health > 50 && player.health < 75) {
		vEmoHealthX = 69 * 2;
	}
	if (player.health > 25 && player.health < 50) {
		vEmoHealthX = 69 * 1;
	}
	if (player.health > 0 && player.health < 25) {
		vEmoHealthX = 69 * 0;
	}
}

void hud_ingame_show() {
	set(panEmoHealth, SHOW);
}

void hud_ingame_hide() {
	reset(panEmoHealth, SHOW);
}
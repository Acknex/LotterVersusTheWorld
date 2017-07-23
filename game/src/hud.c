void hud_ingame_init() {
	if (!fontHud) {
		fontHud = font_create("Arial#20b");
	}
	if (!panEmoHealth) {
		panEmoHealth = pan_create(NULL, 1);
		panEmoHealth.bmap = bmapHealthHud;
		pan_setwindow(panEmoHealth, 0, 25, 15, bmap_width(bmapEmoHealth) / 4, bmap_height(bmapEmoHealth), bmapEmoHealth, vEmoHealthX, 0);
		pan_setdigits(panEmoHealth, 0, bmap_width(bmapHealthHud) / 2, bmap_height(bmapHealthHud) - 37, "%02.f", fontHud, 1, vPlayerHealth);
		pan_setcolor(panEmoHealth, 1, 1, COLOR_RED);
		panEmoHealth.alpha = 80;
		set(panEmoHealth, OUTLINE | TRANSLUCENT | CENTER_X);
	}
	
	hud_ingame_align();
}

void hud_ingame_align() {
	if (panEmoHealth) {
		panEmoHealth.pos_x = 10;
		panEmoHealth.pos_y = screen_size.y - bmap_height(bmapHealthHud);
	}
}

void show_blood_hit() {
	proc_kill((void*)show_blood_hit);
	ColorVariation = 1;
	wait(-0.3);
	ColorVariation = 0;
}

void hud_ingame_update() {
	if (!player) return;
	
	vPlayerHealth = player.health;
	if (vPlayerHealth < vPlayerOldHealth) {
		show_blood_hit();
	}
	vPlayerOldHealth = vPlayerHealth;
	
	if (player.health > 75) {
		vEmoHealthX = bmap_width(bmapEmoHealth) / 4 * 3;
	}
	if (player.health > 50 && player.health < 75) {
		vEmoHealthX = bmap_width(bmapEmoHealth) / 4 * 2;
	}
	if (player.health > 25 && player.health < 50) {
		vEmoHealthX = bmap_width(bmapEmoHealth) / 4 * 1;
	}
	if (player.health > 0 && player.health < 25) {
		vEmoHealthX = bmap_width(bmapEmoHealth) / 4 * 0;
	}
}

void hud_ingame_show() {
	set(panEmoHealth, SHOW);
}

void hud_ingame_hide() {
	reset(panEmoHealth, SHOW);
}
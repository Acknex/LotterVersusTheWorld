#include "weapon.h"
#include "materials.h"

STRING * dialogMessage = "#500";
SOUND* sndDialog = "dialog.wav";

FONT* fontDialog = "Arial#30b";

PANEL * panDialog = 
{
	digits(200,50, "%s", fontDialog, 1, dialogMessage));
	bmap = bmapDialog;
	alpha = 80;
	flags = OUTLINE | TRANSLUCENT | CENTER_X | CENTER_Y;
	layer = 10;
	scale_y = 0.01;
}

void hud_ingame_init() {
	if (!fontHud) {
		fontHud = font_create("Arial#20b");
	}
	if (!fontDialog) {
		fontDialog = font_create("Arial#40b");
	}
	if (!panEmoHealth) {
		panEmoHealth = pan_create(NULL, 1);
		panEmoHealth.bmap = bmapHealthHud;
		pan_setwindow(panEmoHealth, 0, 25, 15, bmap_width(bmapEmoHealth) / 4, bmap_height(bmapEmoHealth), bmapEmoHealth, vEmoHealthX, 0);
		pan_setwindow(panEmoHealth, 0, bmap_width(bmapHealthHud) - 4, bmap_height(bmapHealthHud) - bmap_height(bmapBombSlots) - 11, bmap_width(bmapBombSlots) / 6, bmap_height(bmapBombSlots), bmapBombSlots, vBombCooldown, 0);
		pan_setdigits(panEmoHealth, 0, bmap_width(bmapHealthHud) / 2, bmap_height(bmapHealthHud) - 37, "%02.f", fontHud, 1, vPlayerHealth);
		pan_setcolor(panEmoHealth, 1, 1, COLOR_RED);
		panEmoHealth.alpha = 80;
		set(panEmoHealth, OUTLINE | TRANSLUCENT | CENTER_X);
	}
	
	if (!panDialog) {
		panDialog = pan_create(NULL, 2);
		panDialog.bmap = bmapDialog;
		panDialog.scale_y = 0.01;
		pan_setdigits(panDialog, 0, bmap_width(bmapDialog) / 2, bmap_height(bmapDialog) / 2, "%s", fontDialog, 1, &dialogMessage);
		panDialog.alpha = 80;
		set(panDialog, OUTLINE | TRANSLUCENT | CENTER_X | CENTER_Y);
	}
	
	if(!panPlayerDead) {
		panPlayerDead = pan_create(NULL, 3);
		panPlayerDead.bmap = bmapPlayerDead;
		panPlayerDead.alpha = 0;
		set(panPlayerDead,  TRANSLUCENT | CENTER_X | CENTER_Y);
	}
	
	hud_ingame_align();
}

void show_death_screen(var alpha)
{
	panPlayerDead.alpha = alpha;
	set(panPlayerDead, SHOW);	
}
void hide_death_screen()
{
	reset(panPlayerDead, SHOW);
}

void show_dialog(char * _text) {
	proc_mode = PROC_GLOBAL;
//	str_cpy(dialogMessage, _text);
	if (panDialog) {
		proc_kill(4);
		panDialog.scale_y = 0.01;
		set(panDialog, SHOW);
		snd_play(sndDialog,100,0);
		while(panDialog.scale_y < 1) 
		{
			wait(1);
		//	panDialog.scale_y += 0.1 * time_step;
			panDialog.scale_y = minv(panDialog.scale_y + 0.1 * time_step, 100);
		}
		str_cpy(dialogMessage, _text);
		//panDialog.scale_y = 1;
		// pan_setdigits(panDialog, 1, bmap_width(bmapDialog) / 2, bmap_height(bmapDialog) / 2, "%s", fontDialog, 1, dialogMessage);
		wait(-3);
		// pan_setdigits(panDialog, 1, bmap_width(bmapDialog) / 2, bmap_height(bmapDialog) / 2, "", fontDialog, 1, vDummy);
		str_cpy(dialogMessage, "");
		while(panDialog.scale_y > 0.01) 
		{
			wait(1);
			//panDialog.scale_y -= 0.1 * time_step;
			panDialog.scale_y = maxv(0.01, panDialog.scale_y - 0.1 * time_step);
			
		}
		//panDialog.scale_y = 0;
		reset(panDialog, SHOW);
//		str_cpy(dialogMessage, "");
	}
}

void hide_dialog() {
	if (panDialog) {
		reset(panDialog, SHOW);
	}
}

void hud_ingame_align() {
	if (panEmoHealth) {
		panEmoHealth.pos_x = 10;
		panEmoHealth.pos_y = screen_size.y - bmap_height(bmapHealthHud);
		
		
		PPRingOfImpendingDoom.skill1 = floatv(
			  (panEmoHealth.pos_x + 0.5 * bmap_width(bmapHealthHud))
			/ screen_size.x);
		PPRingOfImpendingDoom.skill2 = floatv(
			  (panEmoHealth.pos_y + 0.5 * bmap_height(bmapHealthHud))
			/ screen_size.y);
		
	}
	if (panDialog) {
		panDialog.pos_x = screen_size.x / 2 - bmap_width(panDialog.bmap) / 2;
		panDialog.pos_y = screen_size.y / 2 - bmap_height(panDialog.bmap) / 2;
	}
	if (panPlayerDead) {
		//panPlayerDead.pos_x = screen_size.x / 2 - bmap_width(panPlayerDead.bmap) / 2;
		//panPlayerDead.pos_y = screen_size.y / 2 - bmap_height(panPlayerDead.bmap) / 2;
		panPlayerDead.pos_x = 0;
		panPlayerDead.pos_y = 0;
		panPlayerDead.scale_x = screen_size.x / bmap_width(bmapPlayerDead);
		panPlayerDead.scale_y = screen_size.y / bmap_height(bmapPlayerDead);
	}
}

void show_blood_hit() {
	proc_kill((void*)show_blood_hit);
	ColorVariation = 1;
		wait(-0.2);
	while(ColorVariation > 0) {
		ColorVariation -=0.2 * time_step;
		wait(1);
	}
	ColorVariation = 0;
}

void hud_red_ring_ping()
{
	// TODO: Play a sound
	var s;
	for(s = 0; s < 6; s += 0.6 * time_step) {
		PPRingOfImpendingDoom.skill3 = floatv(s);
		wait(1);
	}
	PPRingOfImpendingDoom.skill3 = floatv(10);
}

void hud_ingame_update() {
	if (!player) return;
	
	vPlayerHealth = player.health;
	if (vPlayerHealth < vPlayerOldHealth) {
		show_blood_hit();
		if(vPlayerOldHealth > 20 && vPlayerHealth <= 20) {
			hud_red_ring_ping();
		}
	}
	vPlayerOldHealth = vPlayerHealth;
	
	// Player health
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
	
	// Bomb cooldown
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 0) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 0;
	}
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 1) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 1;
	}
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 2) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 2;
	}
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 3) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 3;
	}
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 4) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 4;
	}
	if (player.weapon_granade_cooldown > weapon_grenade_cooldown_time / 6 * 5) {
		vBombCooldown = bmap_width(bmapBombSlots) / 6 * 5;
	}
}

void hud_ingame_show() {
	set(panEmoHealth, SHOW);
}

void hud_ingame_hide() {
	reset(panEmoHealth, SHOW);
	reset(panDialog, SHOW);
}
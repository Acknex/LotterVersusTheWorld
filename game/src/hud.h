#ifndef HUD_H
#define HUD_H

var vEmoHealthX = 0;
var vPlayerHealth = 0;
var vPlayerOldHealth = 0;
var vBombCooldown = 103;
var vDummy = 0;

FONT* fontHud = NULL;

BMAP* bmapEmoHealth = "emohealthsmall.tga";
BMAP* bmapHealthHud = "healthHudx.tga";
BMAP* bmapBombSlots = "bombSlots.tga";
BMAP* bmapDialog = "dialog.tga";
BMAP* bmapPlayerDead = "Gameover_fc.png";

PANEL* panEmoHealth = NULL;
PANEL* panDialog = NULL;

PANEL* panPlayerDead = NULL;

void hud_ingame_init();
void hud_ingame_align();
void hud_ingame_update();

void show_death_screen(var alpha);
void hide_death_screen();

#include "hud.c"

#endif
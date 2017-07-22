#ifndef HUD_H
#define HUD_H

var vEmoHealthX = 0;
var vPlayerHealth = 0;

FONT* fontHud = NULL;

BMAP* bmapEmoHealth = "emohealthsmall.tga";
BMAP* bmapHealthHud = "healthHud.tga";
PANEL* panEmoHealth = NULL;

void hud_ingame_init();
void hud_ingame_align();
void hud_ingame_update();

#include "hud.c"

#endif
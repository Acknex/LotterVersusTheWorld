#ifndef SPLASH_H
#define SPLASH_H

BMAP* SPLASH__logoBmap = "logo.png";
PANEL* SPLASH__logoPanel = NULL;

BMAP* SPLASH__menuStartOffBmap = "menu_start.png";
BMAP* SPLASH__menuStartOnBmap = "menu_start_over.png";
BMAP* SPLASH__menuExitOffBmap = "menu_exit.png";
BMAP* SPLASH__menuExitOnBmap = "menu_exit_over.png";
PANEL* SPLASH__menuPanel = NULL;

ENTITY* SPLASH__cube = NULL;
ENTITY* SPLASH__beam = NULL;
ENTITY* SPLASH__lotti = NULL;

var SPLASH__inSplash = 1;
var SPLASH__logoY = 0;

void SPLASH__init();
void SPLASH__setupLevel();
void SPLASH__animStart();
void SPLASH__animEnd();
void SPLASH__reposition();
int SPLASH__cancelAnim();
void SPLASH__initMenu();
void SPLASH__startGame();
void SPLASH__housekeeping();


#include "splash.c"

#endif
#ifndef SPLASH_H
#define SPLASH_H

BMAP* SPLASH__logoBmap = "logo.png";
PANEL* SPLASH__logoPanel = NULL;

ENTITY* SPLASH__cube = NULL;
ENTITY* SPLASH__beam = NULL;
ENTITY* SPLASH__lotti = NULL;

var SPLASH__inSplash = 1;
var SPLASH__logoY = 0;

void SPLASH__init();
void SPLASH__setupLevel();
void SPLASH__animStart();
void SPLASH__animEnd();
void SPLASH__positionLogo();
int SPLASH__cancelAnim();


#include "splash.c"

#endif
#ifndef SPLASH_H
#define SPLASH_H

BMAP* SPLASH__smokeSprite = "smoke.tga";

BMAP* SPLASH__logoBmap = "logo.png";
PANEL* SPLASH__logoPanel = NULL;

BMAP* SPLASH__menuStartOffBmap = "menu_start.png";
BMAP* SPLASH__menuStartOnBmap = "menu_start_over.png";
BMAP* SPLASH__menuCreditsOffBmap = "menu_credits.png";
BMAP* SPLASH__menuCreditsOnBmap = "menu_credits_over.png";
BMAP* SPLASH__menuExitOffBmap = "menu_exit.png";
BMAP* SPLASH__menuExitOnBmap = "menu_exit_over.png";
PANEL* SPLASH__menuPanel = NULL;

ENTITY* SPLASH__cube = NULL;
ENTITY* SPLASH__beam = NULL;
ENTITY* SPLASH__lotti = NULL;
ENTITY* SPLASH__smokeEmitter = NULL;
ENTITY* SPLASH__introEnt1 = NULL;
ENTITY* SPLASH__introEnt2 = NULL;
ENTITY* SPLASH__introEnt3 = NULL;

var SPLASH__inSplash = 1;
var SPLASH__inIntro = 0;
var SPLASH__logoY = 0;
var SPLASH__blueBias = 0;
var SPLASH__greenBias = 0;
var SPLASH__redBias = 0;

// Menu Stuff

void SPLASH__init();
void SPLASH__setupLevel();

int SPLASH__animStart();
void SPLASH__animEnd();
int SPLASH__cancelAnim();

void SPLASH__reposition();

void SPLASH__initMenu();
void SPLASH__startGame();
void SPLASH__startCredits();
void SPLASH__exitGame();

void SPLASH__housekeeping();

// Intro stuff

void SPLASH__introStart();
void SPLASH__introEnd();
void SPLASH__introCancel();

function SPLASH__act_smoke(ENTITY*);
action SPLASH__act_smokeGen();


#include "splash.c"

#endif
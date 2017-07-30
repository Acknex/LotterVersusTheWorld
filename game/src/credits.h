#ifndef CREDITS_H
#define CREDITS_H

STRING * credits_source = "credits.dat";
var credits_scrollSpeed = 2.98; // 1 Pixel per quant
var credits_speedup = 1;
var credits_speedup_factor = 4;

// Font configuration
STRING * credits_fontName  = "Arial";
STRING * credits_fontFlags = "b";

// Font sizes for h1,h2,h3,other
var credits_hugeFontSize   = 80;
var credits_largeFontSize  = 60;
var credits_mediumFontSize = 40;
var credits_normalFontSize = 30;

// Distance of an image to the left/right screen border
var credits_imageMarginW   = 80;

// Distance of an image to text above/below.
var credits_imageMarginH   = 30;

// Distance of the text to the left/right screen border
var credits_textMarginW    = 60;

// Length of the filmic separator
var credits_filmicSepW     = 16;

// Distance between the two columns of the filmic display
var credits_filmicTextDist = 32;

void credits_start();

void credits_cancel();

void credits_init();

void credits_placeStuff();

action credits_warlock();

#include "credits.c"

#endif

#ifndef CREDITS_H
#define CREDITS_H

STRING * credits_source = "credits.dat";
var credits_scrollSpeed = 1.0; // 1 Pixel per quant

STRING * credits_fontName  = "Arial";
STRING * credits_fontFlags = "b";
var credits_hugeFontSize   = 80;
var credits_largeFontSize  = 60;
var credits_mediumFontSize = 40;
var credits_normalFontSize = 30;

var credits_imageMarginW = 60;
var credits_imageMarginH = 30;

void credits_start();

void credits_cancel();

void credits_init();

#include "credits.c"

#endif
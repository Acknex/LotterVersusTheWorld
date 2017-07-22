#define FONT__DRAWSIZE 15
#define FONT__DRAWALPHA 80

void FONT_create()
{
//	AddFontResource("media//bitwise.ttf");
//	AddFontResource("media//ponde___.ttf");
	AddFontResource("media//monoid.ttf");
}

void FONT_remove()
{
//	RemoveFontResource("media//bitwise.ttf");
//	RemoveFontResource("media//ponde___.ttf");
	RemoveFontResource("media//monoid.ttf");
}

void FONT_scale()
{
	var vHudScale = screen_size.y / 1200;	
	draw_textmode("monoid", 0, FONT__DRAWSIZE * vHudScale, FONT__DRAWALPHA);	
}


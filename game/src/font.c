#define FONT__DRAWSIZE 15
#define FONT__DRAWALPHA 80

void FONT_create()
{
//	AddFontResource("media//bitwise.ttf");
//	AddFontResource("media//ponde___.ttf");
	AddFontResource("media//monoid-regular.ttf");
}

void FONT_remove()
{
//	RemoveFontResource("media//bitwise.ttf");
//	RemoveFontResource("media//ponde___.ttf");
	RemoveFontResource("media//monoid-regular.ttf");
}

void FONT_scale()
{
	var vHudScale = screen_size.y / 1200;	
	draw_textmode("monoid", 0, FONT__DRAWSIZE * vHudScale, FONT__DRAWALPHA);	
}

void FONT_regular()
{
	var vHudScale = screen_size.y / 1200;	
	draw_textmode("monoid", 0, FONT__DRAWSIZE * vHudScale, FONT__DRAWALPHA);	
}

void FONT_big(var alpha)
{
	var vHudScale = screen_size.y / 1200;	
	draw_textmode("monoid", 0, FONT__DRAWSIZE * vHudScale * 1.5, alpha);	
}


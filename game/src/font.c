void FONT_create()
{
	AddFontResource("media//anudaw.ttf");
}

void FONT_remove()
{
	RemoveFontResource("media//anudaw.ttf");
}

void FONT_scale()
{
	var vHudScale = screen_size.y / 1200;	
	draw_textmode("bitwise", 0, 70 * vHudScale, 90);	
}


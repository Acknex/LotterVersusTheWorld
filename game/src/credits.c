#include "credits.h"
#include "splash.h"
#include "level_furniture.h"
#include "camera.h"
#include "materials.h"

/*
	h1: 1 Text, Huge Font, Center
	h2: 1 Text, Large Font, Center
	h3: 1 Text, Medium Font, Center
	c:  1 Text, Normal Font, Center
	l:  1 Text, Normal Font, Left
	r:  1 Text, Normal Font, Right
	x:  2 Text, Normal Font, (Left,Right)
	f:  2 Text, Normal Font, (Left,'-',Right)
	i:  1 File, Autosize, Center
	s:  1 Number, Spacing in num * normal font size
*/

typedef struct CreditsNode
{
	var height;
	var alignment; /* 0=left, 1=middle, 2=right */
	int fuckYouType; /* 0=text, 1=image, 2=empty, 3=filmic */
	STRING * text1;
	STRING * text2;
	BMAP   * image;
	int font;
	struct CreditsNode * next;
} CreditsNode;

CreditsNode * credits_firstNode = NULL;

FONT * credits_fontset[4];

TEXT * credits_text = 
{
	strings = 1;
	string ( "This is stupid!");
}

BMAP* credits_bmp_ff = "credits_ff.tga";

PANEL* credits_pnl_ff =
{
	pos_x = 0;
	pos_y = 0;
	size_x = 32;
	size_y = 16;
	bmap = credits_bmp_ff;

//	flags = SHOW;
}

void credits_init()
{
	credits_imageMarginW = screen_size.x * 0.2;
	credits_textMarginW  = screen_size.x * 0.2;
	
	STRING * temp = "#100";
	credits_fontset[0] = font_create(str_printf(temp,
		"%s#%d%s",
		_chr(credits_fontName),
		(int)credits_normalFontSize,
		_chr(credits_fontFlags)));
	credits_fontset[1] = font_create(str_printf(temp,
		"%s#%d%s",
		_chr(credits_fontName),
		(int)credits_mediumFontSize,
		_chr(credits_fontFlags)));
	credits_fontset[2] = font_create(str_printf(temp,
		"%s#%d%s",
		_chr(credits_fontName),
		(int)credits_largeFontSize,
		_chr(credits_fontFlags)));
	credits_fontset[3] = font_create(str_printf(temp,
		"%s#%d%s",
		_chr(credits_fontName),
		(int)credits_hugeFontSize,
		_chr(credits_fontFlags)));
	
	if(credits_fontset[0] == NULL) error("Failed to init font 0");
	if(credits_fontset[1] == NULL) error("Failed to init font 1");
	if(credits_fontset[2] == NULL) error("Failed to init font 2");
	if(credits_fontset[3] == NULL) error("Failed to init font 3");
	
	var f = file_open_read(credits_source);
	
	STRING * tempString = "#512";
	STRING * fuckYouTypeString = "#512";
	STRING * inputString = "#512";

	str_cpy(delimit_str, ";\n");
	
	diag("\nLoading credits data...");
	
	CreditsNode * it = NULL;
	while(TRUE)
	{
		if(file_str_readto(f, fuckYouTypeString, NULL, 512) < 0) {
			break;
		}
		CreditsNode * curr = malloc(sizeof(CreditsNode));
		memset(curr, 0, sizeof(CreditsNode));
		
		if(str_cmp("h1", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.font = 3;
			curr.text1 = str_create(inputString);
			curr.alignment = 1;
		}
		else if(str_cmp("h2", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.font = 2;
			curr.text1 = str_create(inputString);
			curr.alignment = 1;
		}
		else if(str_cmp("h3", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.font = 1;
			curr.text1 = str_create(inputString);
			curr.alignment = 1;
		}
		else if(str_cmp("c", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.text1 = str_create(inputString);
			curr.alignment = 1;
		}
		else if(str_cmp("l", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.text1 = str_create(inputString);
			curr.alignment = 0;
		}
		else if(str_cmp("r", fuckYouTypeString)) {
			file_str_read(f, inputString);
			curr.fuckYouType = 0;
			curr.text1 = str_create(inputString);
			curr.alignment = 2;
		}
		else if(str_cmp("x", fuckYouTypeString)) {
			curr.fuckYouType = 0;
			
			file_str_read(f, inputString);
			curr.text1 = str_create(inputString);
			
			file_str_read(f, inputString);
			curr.text2 = str_create(inputString);
			
			curr.alignment = 0;
		}
		else if(str_cmp("f", fuckYouTypeString)) {
			curr.fuckYouType = 3;
			
			file_str_read(f, inputString);
			curr.text1 = str_create(inputString);
			
			file_str_read(f, inputString);
			curr.text2 = str_create(inputString);
			
			curr.alignment = 0;
		}
		else if(str_cmp("i", fuckYouTypeString)) {
			curr.fuckYouType = 1;
			
			file_str_read(f, inputString);
			curr.image = bmap_create(inputString);
			if(curr.image == NULL) {
				str_printf(
					tempString,
					"Image %s not found!",
					_chr(inputString));
				error(tempString);
			}
			
			curr.alignment = 1;
		}
		else if(str_cmp("s", fuckYouTypeString)) {
			curr.fuckYouType = 2;
			file_str_read(f, inputString);
			curr.height = credits_normalFontSize * str_to_num(inputString);
		}
		else {
			str_printf(
				tempString,
				"Unknown directive recognized: %s",
				_chr(fuckYouTypeString));
			error(tempString);
		}
		
		if(curr != NULL) {
			diag_var("\nnode(%f)", (var)curr.fuckYouType);
			if(it != NULL ) {
				it->next = curr;
			} else {
				credits_firstNode = curr;
			}
			it = curr;
		}
	}
	
	file_close(f);
}

void credits_run()
{
	var timer = 0;
	while(TRUE)
	{
		CreditsNode * it;
		var h = 0;
		var y = screen_size.y - timer;
		
		for(it = credits_firstNode; it != NULL; it = it->next, y += h)
		{
			if(y >= screen_size.y) {
				break;
			}
			h = it.height;
			COLOR * color = COLOR_RED;
			switch(it->fuckYouType)
			{
				case 0: // text
					credits_text.font = credits_fontset[it.font];
					credits_text.pos_y = y;
					
					credits_text.flags &= ~(CENTER_X | ARIGHT);
					switch(it.alignment) {
						case 0:
							credits_text.pos_x = credits_textMarginW;
						case 1:
							credits_text.pos_x = 0.5 * screen_size.x;
							credits_text.flags |= CENTER_X;
							break;
						case 2:
							credits_text.pos_x = screen_size.x - credits_textMarginW;
							credits_text.flags |= ARIGHT;
							break;
					}
					
					if(it.text1) {
						(credits_text.pstring)[0] = it.text1;
						draw_obj(credits_text);
					}
					
					if(it.text2) {
						credits_text.pos_x = screen_size.x - credits_textMarginW;
						credits_text.flags |= ARIGHT;
						(credits_text.pstring)[0] = it.text2;
						draw_obj(credits_text);
					}
					
					y += credits_text.font.dy;
					break;
				case 1: // image
					h = bmap_height(it.image);
					var w = bmap_width(it.image);
					var sc = (screen_size.x - 2*credits_imageMarginW) / w;
					draw_quad(
						it.image,
						vector(
							0.5 * (screen_size.x - sc * w),
							y + credits_imageMarginH,
							0),
						NULL,
						vector(w, h, 0), vector(sc, sc, 0),
						NULL, 100, 0);
					h *= sc;
					h += 2 * credits_imageMarginH;
					break;
				case 2: // space
					break;
					
				case 3: // filmic
					credits_text.font = credits_fontset[it.font];
					credits_text.pos_y = y;
					
					credits_text.flags &= ~(CENTER_X | ARIGHT);
					credits_text.flags |= ARIGHT;
					credits_text.pos_x = 0.5 * screen_size.x - 0.5 * credits_filmicTextDist;
					(credits_text.pstring)[0] = it.text1;
					draw_obj(credits_text);
					
					draw_quad(
						NULL,
						vector(
							0.5 * screen_size.x - 0.5 * credits_filmicSepW,
							y + 0.5 * credits_text.font.dy,
							0),
						NULL,
						vector(credits_filmicSepW, 1, 0),
						NULL,
						COLOR_WHITE,
						100,
						0);
					
					credits_text.flags &= ~(CENTER_X | ARIGHT);
					credits_text.pos_x = 0.5 * screen_size.x + 0.5 * credits_filmicTextDist;
					(credits_text.pstring)[0] = it.text2;
					draw_obj(credits_text);
					
					y += credits_text.font.dy;
					break;
				default:
					error(str_printf(NULL, "invalid fuckYouType: %d", it.fuckYouType));
					break;
			}
		}
		if(y <= 0) {
			break;
		}
		
		
		if(key_space)
		{
			credits_speedup = credits_speedup_factor;

			if ((total_ticks % 16) > 8)
			{
				credits_pnl_ff.flags |= SHOW;
			}
			else
			{
				credits_pnl_ff.flags &= ~SHOW;
			}
		}
		else
		{
			credits_speedup = 1;
			credits_pnl_ff.flags &= ~SHOW;
		}
		timer += credits_speedup * credits_scrollSpeed * time_step;

		// Rotate Camera during credits
		VIEW* cam = get_camera();
		cam.x =0; cam.y=0; cam.z=100;
		cam->pan += time_step*0.2*credits_speedup;

		speedupMusic(100*credits_speedup);
		
		wait(1);
	}
	
	// Wait a bit, then terminate the credits
	startMusic(NULL, 0.5, 0);
	while(playingMusic() == 1)
	{
		wait(1);
	}
	
	// TODO:;
	// Return to main menu here
	credits_cancel();
}

void credits_start()
{
	if(credits_firstNode == NULL) {
		error("Error in credits: failed to initialize credits!");
		return;
	}

	credits_pnl_ff.scale_x = (screen_size.x/16)/32;
	credits_pnl_ff.scale_y = (screen_size.y/16)/16;

	//error(str_for_num(NULL, credits_pnl_ff.scale_x));

	//credits_pnl_ff.scale_x = 10;
	//credits_pnl_ff.scale_y = 10;

	create_camera();
	show_camera();
	
	// Kill all evil inthis world!
	level_load("credits.wmb");
	credits_placeStuff();
	
	pp_bloom_start(2.5);

	// Use normal entity for
	// running the credits in the
	// background
	ent_create(NULL, vector(0,0,0), credits_run);
	
	startMusic("media\\credits.mp3", 0.5, 0);
}

void credits_cancel()
{
	speedupMusic(100);
	level_load(NULL);
	remove_camera();
	credits_pnl_ff.flags &= ~SHOW;
	// TODO: Replace with "return-to-menu"!
	SPLASH__init();
}

void credits_placeStuff()
{
	int i;
	for (i = 0; i < 360; i+=30)
	{
		var posy=sinv(i)*450;
		var posx=cosv(i)*450;
		var r=random(100);
		if ((r>=0) && (r<60))
		{
			ENTITY* desk = ent_create("desk.mdl", vector(posx, posy, 0), desk_buildup);
			desk->skill1 = i+180;
		}
		else if ((r>=60) && (r<90))
		{
			ENTITY* screen = ent_create("screen.mdl", vector(posx, posy, 0), screenOnWall);
			screen->pan = i+180;

		}
		else
		{
			ENTITY* rack = ent_create("rack_case.mdl", vector(posx, posy, 0), rack_buildup);
			rack->skill1 = i+180;
		}
	}
}

action credits_warlock()
{
	ENTITY* keyboard = ent_create("rack_console_keyboard.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(-11, -3, 68), _vec(my->skill1, 0, 0))), 0);

	while(me)
	{
		my->skill1 = (my->skill1 + time_step*6)%100;
		ent_animate(me, "duck", my->skill1, ANM_CYCLE);
		DEBUG_VAR(my->skill1,20);
		wait(1);
	}
}


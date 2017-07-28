#include "credits.h"

/*
	h1: 1 Text, Huge Font, Center
	h2: 1 Text, Large Font, Center
	h3: 1 Text, Medium Font, Center
	c:  1 Text, Normal Font, Center
	l:  1 Text, Normal Font, Left
	r:  1 Text, Normal Font, Right
	x:  2 Text, Normal Font, (Left,Right)
	i:  1 File, Autosize, Center
	s:  1 Number, Spacing in num * normal font size
*/

typedef struct CreditsNode
{
	var height;
	var alignment; /* 0=left, 1=middle, 2=right */
	int fuckYouType; /* 0=text, 1=image, 2=empty */
	STRING * text1;
	STRING * text2;
	BMAP   * image;
	int font;
	struct CreditsNode * next;
} CreditsNode;

CreditsNode * credits_firstNode = NULL;

var credits_musicHandle;

FONT * credits_fontset[4];

TEXT * credits_text = 
{
	strings = 1;
	string ( "This is stupid!");
}

void credits_init()
{
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
							credits_text.pos_x = 32;
						case 1:
							credits_text.pos_x = 0.5 * screen_size.x;
							credits_text.flags |= CENTER_X;
							break;
						case 2:
							credits_text.pos_x = screen_size.x - 32;
							credits_text.flags |= ARIGHT;
							break;
					}
					
					if(it.text1) {
						(credits_text.pstring)[0] = it.text1;
						draw_obj(credits_text);
					}
					
					if(it.text2) {
						credits_text.pos_x = screen_size.x - 32;
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
				default:
					error(str_printf(NULL, "invalid fuckYouType: %d", it.fuckYouType));
					break;
			}
		}
		if(y <= 0) {
			break;
		}
		
		var speedup = 1;
		if(key_space) {
			speedup = 10;
		}
		timer += speedup * credits_scrollSpeed * time_step;
		wait(1);
	}
	
	// Wait a bit, then terminate the credits
	while(media_playing(credits_musicHandle))
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
	
	// Kill all evil inthis world!
	level_load(NULL);
	
	// Use normal entity for
	// running the credits in the
	// background
	ent_create(NULL, vector(0,0,0), credits_run);
	
	credits_musicHandle = media_play(credits_song, NULL, 100);
}

void credits_cancel()
{
	level_load(NULL);
	media_stop(credits_musicHandle);
	// TODO: Replace with "return-to-menu"!
	sys_exit("Credits exit!");
}
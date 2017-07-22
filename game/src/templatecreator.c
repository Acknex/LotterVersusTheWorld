///////////////////////////////
#include <acknex.h>
#include <default.c>
//#include "..\\src\\datatypes.h"
//#include "..\\src\\levelgen.h"
///////////////////////////////

// used by stageFill to create differently looking levels
struct _TEMPLATE
{
	char cfile[32];
	int size[2];
	int *values;
};

typedef struct _TEMPLATE TEMPLATE;



TEMPLATE* templateCreate(char* pcfile, int sizeX, int sizeY)
{
	int i,j;
	var fhandle;
	TEMPLATE* template;
	
	template = (TEMPLATE*)sys_malloc(sizeof(TEMPLATE));
	strcpy(&(template->cfile),pcfile);
	template->size[0] = sizeX;
	template->size[1] = sizeY;
	template->values = (int*)sys_malloc(sizeof(int)*sizeX*sizeY);
	
	fhandle = file_open_read(pcfile);
	if(fhandle)
	{
		if(template->size[0] != file_var_read(fhandle) || template->size[1] != file_var_read(fhandle)) printf("templateCreate: incompatible sizes: (%d - %d,  %d - %d)",sizeX,template->size[0],sizeY,template->size[1]);
		for(i = 0; i < template->size[0]; i++)
		{
			for(j = 0; j < template->size[1]; j++) (template->values)[i*template->size[1]+j] = file_var_read(fhandle);
		}			
		file_close(fhandle);	
	}
	else memset(template->values,0,sizeX*sizeY);
	
	return template;
}

void templateDestroy(TEMPLATE* template)
{
	if(template->values) sys_free(template->values);
	sys_free(template);
}

void templateSave(TEMPLATE* template, char* pcfile)
{
	int i,j;
	var fhandle;
	
	fhandle = file_open_write(pcfile);
	if(fhandle)
	{
		file_var_write(fhandle,template->size[0]);
		file_var_write(fhandle,template->size[1]);
		for(i = 0; i < template->size[0]; i++)
		{
			for(j = 0; j < template->size[1]; j++) file_var_write(fhandle,(template->values)[i*template->size[1]+j]);
		}			
		file_close(fhandle);	
	}
}

TEMPLATE* currentTemplate = NULL;

TEXT* input_txt =
{
	pos_x = 20;
	pos_y = 20;
	string("Press [1] to enter a filename. [2] to save, [3] to load.","                        ");
	flags = SHOW;
}

void main()
{
	fps_max = 120;
	video_mode = 9;
	level_load(NULL);
	mouse_mode = 4;
	while(1)
	{
		int i,j;
		VECTOR vColor;
		
		static int key_1_off = 0;
		if(key_1)
		{
			if(key_1_off)
			{
				key_1_off = 0;
				inkey((input_txt.pstring)[1]);
				if(currentTemplate) templateDestroy(currentTemplate);
				currentTemplate = templateCreate((input_txt.pstring)[1], 24, 24);
			}
		}
		else key_1_off = 1;

		static int key_2_off = 0;
		if(key_2)
		{
			if(key_2_off)
			{
				key_2_off = 0;
				if(currentTemplate) templateSave(currentTemplate, (input_txt.pstring)[1]);
			}
		}
		else key_2_off = 1;

		static int key_3_off = 0;
		if(key_3)
		{
			if(key_3_off)
			{
				key_3_off = 0;
				if(currentTemplate) templateDestroy(currentTemplate);
				currentTemplate = templateCreate((input_txt.pstring)[1], 24, 24);
			}
		}
		else key_3_off = 1;
		
		if(currentTemplate)
		{
			draw_text("adsjajsd",40,100,COLOR_RED);
			for(i = 0; i < currentTemplate->size[0]; i++)
			{
				for(j = 0; j < currentTemplate->size[1]; j++)
				{
					if((currentTemplate->values)[i*currentTemplate->size[1]+j]) vec_fill(vColor,128);
					else vec_fill(vColor,16);
					draw_quad(NULL, vector(20+i*16,80+j*16,0), NULL, vector(16,16,0), NULL, vColor, 100, 0);
				}		
			}
			if(mouse_left || mouse_right)
			{
				i = floor((mouse_pos.x-20)/16.0);
				j = floor((mouse_pos.y-80)/16.0);
				if(i >= 0 && i < currentTemplate->size[0] && j >= 0 && j < currentTemplate->size[1])
				{
					(currentTemplate->values)[i*currentTemplate->size[1]+j] = 1-mouse_right;
				}
			}
		}
		
		wait(1);
	}
}



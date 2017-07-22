
#ifndef LEVELGEN_H
	#define LEVELGEN_H

	#include "datatypes.h"
	#include "console.h"

	int levelgenOffset2D[8] = { -1,0, 0,1, 1,0, 0,-1 };

	LEVELGENSTACK* levelgenStackCreate(int size);

	TEMPLATE* templateCreate(STRING* str_file, int sizeX, int sizeY);

	void templateDestroy(TEMPLATE* template);

	void templateSave(TEMPLATE* template, char* pcfile);

	ENTITY * stageRenderEntity(STAGE * stage);

	VECTOR * stage_load(STAGE * stage);
	void stage_unload();

	STAGE* stageCreate(int sizeX, int sizeY, var seed, int level);
	void stageDestroy(STAGE* stage);

	TILE* stageGetTile(STAGE* stage, int i, int j);
	
	VECTOR* stageGetPosFromIndices(STAGE* stage, VECTOR* vpos, int x, int y);

	VECTOR* stageGetExitPos(STAGE* stage, VECTOR* vpos, int *px, int *py);

	VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py);

	// create a random level, not traversable
	void stageFill(STAGE* stage);

	// make level traversable
	void stageConnect(STAGE* stage);

	void stageDoFlood(STAGE* stage, int startX, int startY, int floodId, int floodMax, int mode);
	
	void stageAddExitAndEntrance(STAGE* stage);
	
	int stageCreateEnemyIsPositionValid(STAGE* stage, int i2, int j2);
	
	void stageCreateEnemyData(STAGE* stage);
	
	// - for enemy placement:
	
	int stageGetNumEnemies(STAGE* stage);
	
	VECTOR* stageGetEnemyPosition(STAGE* stage, int idx);
	
	// set a flag from datatypes.h after choosing an enemy, for example TILE_FLAG_TRAP_TURRET
	void stageSetEnemyType(STAGE* stage, int idx, int flagType);
	// -

	void stageDraw(STAGE* stage, int posX, int posY, int tileSize);



void draw_line3d2(VECTOR* vfrom, VECTOR* vto, COLOR* color, var alpha)
{
	draw_line3d(vfrom, NULL, alpha);
	draw_line3d(vfrom, color, alpha);
	draw_line3d(vto, color, alpha);
}

void draw_num3d(var num, VECTOR* vpos, var yOffset, COLOR* color)
{
	STRING* str;
	VECTOR temp;
	
	vec_set(temp,vpos);
	if(vec_to_screen(temp,get_camera()))
	{
		str = str_for_num(NULL,num);
		draw_text(str,temp.x-5,temp.y-5+yOffset,COLOR_BLACK);
		draw_text(str,temp.x-4,temp.y-4+yOffset,color);
	}
}

void draw_str3d(STRING* str, VECTOR* vpos, var yOffset, COLOR* color)
{
	VECTOR temp;
	
	vec_set(temp,vpos);
	if(vec_to_screen(temp,get_camera()))
	{
		draw_text(str,temp.x-5,temp.y-5+yOffset,COLOR_BLACK);
		draw_text(str,temp.x-4,temp.y-4+yOffset,color);
	}
}

STAGE* LEVEL__stage = NULL;

	#include "levelgen.c"

#endif
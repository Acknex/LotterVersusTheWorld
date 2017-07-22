
#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "datatypes.h"

int levelgenOffset2D[8] = { -1,0, 0,1, 1,0, 0,-1 };

void stage_load(STAGE * stage);

STAGE* stageCreate(int sizeX, int sizeY, var seed)
{
	STAGE* stageNew;
	
	stageNew = (STAGE*)sys_malloc(sizeof(STAGE));
	if(seed) stageNew->seed = seed;
	else
	{
		random_seed(0);
		stageNew->seed = random(10000);
	}
	random_seed(stageNew->seed);
	stageNew->size[0] = sizeX;
	stageNew->size[1] = sizeY;
	stageNew->tiles = (TILE*)sys_malloc(sizeof(TILE)*sizeX*sizeY);
	
	return stageNew;
}

TILE* stageGetTile(STAGE* stage, int i, int j)
{
	if(i < 0 || i >= stage->size[0] || j < 0 || j >= stage->size[1]) return NULL;
	return &(stage->tiles)[i*stage->size[1]+j];
}

// create a random level, not traversable
void stageFill(STAGE* stage)
{
	int i,j,k,i2,j2,count;
	TILE* tile, *tile2;
	
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			if(!i || i >= stage->size[0]-1 || !j || j >= stage->size[1]-1) tile->value = 1;
			else
			{
				tile->value = random(2);
			}
		}		
	}
	
	// remove noise (empty 1x1 areas)
	for(i = 1; i < stage->size[0]-1; i++)
	{
		for(j = 1; j < stage->size[1]-1; j++)
		{
			tile = stageGetTile(stage,i,j);
			if(!tile->value)
			{
				// count solid surrounding tiles
				count = 0;
				for(k = 0; k < 4; k++)
				{
					i2 = i+levelgenOffset2D[k*2+0];
					j2 = j+levelgenOffset2D[k*2+1];
					tile2 = stageGetTile(stage,i2,j2);
					if(tile2->value) count++;
				}
				if(count == 4) tile->value = 2;
			}		
		}		
	}
}

// make level traversable
void stageConnect(STAGE* stage)
{
	int i,j;
	TILE* tile;
	
	/*for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			
			if(!i || i >= stage->size[0]-1 || !j || j >= stage->size[1]-1) tile->value = 1;
			else
			{
				tile->value = random(2);
			}
		}		
	}*/
}


void stageDraw(STAGE* stage, int posX, int posY, int tileSize)
{
	int i,j;
	TILE* tile;
	VECTOR vColor;
	
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			if(tile->value)
			{
				vec_fill(vColor,128);
				//if(tile->value == 2) vec_set(vColor,vector(0,0,255));
			}
			else vec_fill(vColor,16);
			draw_quad(NULL, vector(posX+i*tileSize,posY+j*tileSize,0), NULL, vector(tileSize,tileSize,0), NULL, vColor, 100, 0);
		}		
	}
}

#include "levelcreator.c"

#endif
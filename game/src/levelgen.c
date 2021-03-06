
#include "datatypes.h"
#include "console.h"

LEVELGENSTACK* levelgenStackCreate(int size)
{
	LEVELGENSTACK* newStack;
	
	newStack = (LEVELGENSTACK*)sys_malloc(sizeof(LEVELGENSTACK));
	newStack->stackSize = size;
	newStack->stackCurrent = 0;
	newStack->stackMax = 0;
	newStack->values = (int*)sys_malloc(sizeof(int)*size);
	
	return newStack;
}

TEMPLATE* templateCreate(STRING* str_file, int sizeX, int sizeY)
{
	int i,j;
	var fhandle;
	TEMPLATE* template;
	
	template = (TEMPLATE*)sys_malloc(sizeof(TEMPLATE));
	strcpy((template->cfile),str_file->chars);
	template->size[0] = sizeX;
	template->size[1] = sizeY;
	template->values = (int*)sys_malloc(sizeof(int)*sizeX*sizeY);
	
	fhandle = file_open_read(str_file);
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

STAGE* stageCreate(int sizeX, int sizeY, var seed, int level)
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
	stageNew->level = level;
	stageNew->size[0] = sizeX;
	stageNew->size[1] = sizeY;
	stageNew->tiles = (TILE*)sys_malloc(sizeof(TILE)*sizeX*sizeY);
	memset(stageNew->tiles,0,sizeX*sizeY);
	stageNew->workingStack = levelgenStackCreate(sizeX*sizeY*2);
	stageNew->numWalkableTiles = 0;
	stageNew->numEnemies = 0;
	stageNew->enemyPositions = NULL;
	stageNew->enemyData = NULL;
	stageNew->floodExitMax = 0;
	
	return stageNew;
}

void stageDestroy(STAGE* stage)
{
	if(stage->enemyPositions) sys_free(stage->enemyData);
	if(stage->enemyData) sys_free(stage->enemyData);
	sys_free(stage->tiles);
	sys_free(stage);
}


TILE* stageGetTile(STAGE* stage, int i, int j)
{
	if(i < 0 || i >= stage->size[0] || j < 0 || j >= stage->size[1]) return NULL;
	return &(stage->tiles)[i*stage->size[1]+j];
}

var stageGetIndicesFromPos(STAGE* stage, VECTOR* vpos, int *px, int *py)
{
	int x,y;
	
	x = (vpos->x+100)/200;
	y = (vpos->y+100)/200;
	*px = x;
	*py = y;
	if(x >= 0 && x < stage->size[0] && y >= 0 && y < stage->size[1]) return 1;
	return 0;
}

var stageGetFloodAtPos(STAGE* stage, VECTOR* vpos, int floodId)
{
	int x,y;
	TILE* tile;
	
	if(floodId < 0  || floodId >= FLOOD_TYPE_MAX) error("stageGetFloodAtPos(): bad floodId!");
	if(stageGetIndicesFromPos(stage,vpos,&x,&y))
	{
		tile = stageGetTile(stage,x,y);
		return (tile->flood[floodId]);
	}
	return FLOOD_VALUE_MAX;
}

var stageGetTargetFromFlood(STAGE* stage, VECTOR* vpos, VECTOR* vFinalTarget, VECTOR* vNewTarget, int floodId, var border, var tryStraightLine)
{
	VECTOR currentCenter;
	TILE* tile, *tile2;
	int x,y,finalX,finalY,k,i,j,minX,maxX,minY,maxY,ok;
	
	stageGetIndicesFromPos(stage,vpos,&x,&y);
	stageGetIndicesFromPos(stage,vFinalTarget,&finalX,&finalY);
	//draw_str3d(str_printf(NULL,"(%d, %d) - (%d, %d)",x,y,finalX,finalY),my.x,-80,COLOR_WHITE);
	if(x == finalX && y == finalY)
	{
		vec_set(vNewTarget,vFinalTarget);
		return 0;
	}
	minX = minv(x,finalX);
	maxX = maxv(x,finalX);
	minY = minv(y,finalY);
	maxY = maxv(y,finalY);
	
	if(tryStraightLine == 1)
	{
		ok = 1;

		for(i = minX; i <= maxX; i++)
		{
			for(j = minY; j <= maxY; j++)
			{
				tile = stageGetTile(stage,i,j);
				if(!tile)
				{
					i = maxX+1;
					ok = 0;
					break;
				}
				if(tile->value == TILE_EMPTY)
				{
					i = maxX+1;
					ok = 0;
					break;
				}
			}	
		}
		if(ok)
		{
			vec_set(vNewTarget,vFinalTarget);
			return 0;
		}
	}
	
	currentCenter.x = x*200;
	currentCenter.y = y*200;
	tile = stageGetTile(stage,x,y);
	if(!tile) return FLOOD_VALUE_MAX;
	if(abs(vpos->x-currentCenter.x) < border && abs(vpos->y-currentCenter.y) < 100-border)
	{
		for(k = 0; k < 4; k++)
		{
			i = x+levelgenOffset2D[k*2+0];
			j = y+levelgenOffset2D[k*2+1];
			tile2 = stageGetTile(stage,i,j);
			if(tile2)
			{
				if(tile2->value != TILE_EMPTY && tile2->flood[floodId] < tile->flood[floodId])
				{
					vNewTarget.x = i*200;
					vNewTarget.y = j*200;
					return (tile2->flood[floodId]);
				}
			}
		}
	}
	return (tile->flood[floodId]);
}

VECTOR* stageGetPosFromIndices(STAGE* stage, VECTOR* vpos, int x, int y)
{
	if(vpos)
	{
		vec_set(vpos,vector(x*200,y*200,0));
		return vpos;
	}
	return vector(x*200,y*200,0);
}

VECTOR* stageGetExitPos(STAGE* stage, VECTOR* vpos, int *px, int *py)
{
	if(px) *px = stage->exitPos[0];
	if(py) *py = stage->exitPos[1];
	return stageGetPosFromIndices(stage,vpos,stage->exitPos[0],stage->exitPos[1]);
}

VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py)
{
	if(px) *px = stage->entrancePos[0];
	if(py) *py = stage->entrancePos[1];
	return stageGetPosFromIndices(stage,vpos,stage->entrancePos[0],stage->entrancePos[1]);
}


// create a random level, not traversable
void stageFill(STAGE* stage)
{
	int i,j,k,i2,j2,i3,j3,count,curIter,maxIter,borderValues[4];
	int sourceX,sourceY,targetX,targetY,sizeX,sizeY,numWalkable;
	TILE *tile, *tile2, *borderTiles[4];
	TEMPLATE* template;
	
	template = templateCreate(str_printf(NULL,"level\\template%d.dat",(int)(random(5))), 24, 24); //
	//cprintf1("\ntemplate: %s",template->cfile);
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			tile->value = TILE_EMPTY;
		}		
	}
	maxIter = 30+random(20);
	for(curIter = 0; curIter < maxIter; curIter++)
	{
		sourceX = random(template->size[0]);
		sourceY = random(template->size[1]);
		targetX = random(stage->size[0]);
		targetY = random(stage->size[1]);
		sizeX = random(6)+6;
		sizeY = random(6)+6;
		for(i = 0; i < sizeX; i++)
		{
			for(j = 0; j < sizeY; j++)
			{
				i3 = (targetX+i)%stage->size[0];
				j3 = (targetY+j)%stage->size[1];
				if(i3 > 0 && i3 < stage->size[0]-1 && j3 > 0 && j3 < stage->size[1]-1)
				{
					tile = stageGetTile(stage,i3,j3);
					i2 = (sourceX+i)%template->size[0];
					j2 = (sourceY+j)%template->size[1];
					if(random(3) > 1) tile->value = maxv(tile->value,(template->values)[i2*template->size[1]+j2]);
					else tile->value = (template->values)[i2*template->size[1]+j2];
				}
			}		
		}
	}
	templateDestroy(template);
	
	// remove noise (empty 1x1 areas)
	for(curIter = 0; curIter < 4; curIter++)
	{
		for(i = 1; i < stage->size[0]-1; i++)
		{
			for(j = 1; j < stage->size[1]-1; j++)
			{
				tile = stageGetTile(stage,i,j);
				count = 0;
				for(k = 0; k < 4; k++)
				{
					i2 = i+levelgenOffset2D[k*2+0];
					j2 = j+levelgenOffset2D[k*2+1];
					tile2 = stageGetTile(stage,i2,j2);
					borderTiles[k] = tile2;
					borderValues[k] = tile2->value;
					if(tile2->value == TILE_EMPTY) count++;
				}
				if(tile->value && count == 4) tile->value = TILE_EMPTY;
				if(tile->value == TILE_EMPTY && count <= 1) tile->value = 1;
				if(tile->value && count == 3) tile->value = TILE_EMPTY;
				if(tile->value && count == 1) tile->value = 1;
			}		
		}
	}
}

// make level traversable
void stageConnect(STAGE* stage)
{
	int i,j,k,i2,j2,i3,j3,iterationCount = 0,numComponents,prevStackMax,found;
	TILE* tile, *tile2, *tile3;
	LEVELGENSTACK* workingStack;
	
	workingStack = stage->workingStack;
	
	while(iterationCount < 50000)
	{
		for(i2 = 0; i2 < stage->size[0]; i2++)
		{
			for(j2 = 0; j2 < stage->size[1]; j2++)
			{
				tile = stageGetTile(stage,i2,j2);
				tile->tmp = 0;
				tile->prev[0] = -1;
				tile->prev[1] = -1;
			}		
		}
		numComponents = 0;
		for(i = 1; i < stage->size[0]-1; i++)
		{
			for(j = 1; j < stage->size[1]-1; j++)
			{
				tile = stageGetTile(stage,i,j);
				if(tile->value && !tile->tmp)
				{
					if(numComponents >= 1)
					{
						i = stage->size[0];
						numComponents++;
						break;
					}
					// get first component
					(workingStack->values)[0] = i;
					(workingStack->values)[1] = j;
					workingStack->stackCurrent = 0;
					workingStack->stackMax = 1;
					while(workingStack->stackCurrent < workingStack->stackMax)
					{
						i2 = (workingStack->values)[workingStack->stackCurrent*2+0];
						j2 = (workingStack->values)[workingStack->stackCurrent*2+1];
						for(k = 0; k < 4; k++)
						{
							i3 = i2+levelgenOffset2D[k*2+0];
							j3 = j2+levelgenOffset2D[k*2+1];
							tile2 = stageGetTile(stage,i3,j3);
							if(tile2->value && !tile2->tmp)
							{
								tile2->tmp = 1;
								if(workingStack->stackMax >= workingStack->stackSize) error("stageConnect(): stack overflow! Wow!");
								(workingStack->values)[workingStack->stackMax*2+0] = i3;
								(workingStack->values)[workingStack->stackMax*2+1] = j3;
								workingStack->stackMax++;
							}
						}
						workingStack->stackCurrent++;
					}
					numComponents++;
				}
			}
		}
		
		if(numComponents <= 1) break;
		
		found = 0;
		prevStackMax = workingStack->stackMax;
		workingStack->stackCurrent = 0;
		while(workingStack->stackCurrent < workingStack->stackMax)
		{
			i2 = (workingStack->values)[workingStack->stackCurrent*2+0];
			j2 = (workingStack->values)[workingStack->stackCurrent*2+1];
			tile = stageGetTile(stage,i2,j2);
			//if(tile->value == TILE_EMPTY) error("stageConnect(): iterating over empty tile in pre connection stage ~~~~ ");
			for(k = 0; k < 4; k++)
			{
				i3 = i2+levelgenOffset2D[k*2+0];
				j3 = j2+levelgenOffset2D[k*2+1];
				tile2 = stageGetTile(stage,i3,j3);
				if(i3 > 0 && i3 < stage->size[0]-1 && j3 > 0 && j3 < stage->size[1]-1)
				{
					// move across impassable areas to find a connection between components
					if(tile2->value == TILE_EMPTY && !tile2->tmp)
					{
						tile2->prev[0] = i2;
						tile2->prev[1] = j2;
						tile2->tmp = 2;
						if(workingStack->stackMax >= workingStack->stackSize) error("stageConnect(): stack overflow 2! Wow!");
						(workingStack->values)[workingStack->stackMax*2+0] = i3;
						(workingStack->values)[workingStack->stackMax*2+1] = j3;
						workingStack->stackMax++;
					}
					// found a second component?
					if(tile2->value && !tile2->tmp)
					{
						found = 1;
						workingStack->stackCurrent = workingStack->stackMax;
						break;
					}
				}
			}
			workingStack->stackCurrent++;
		}
		if(!found) error("stageConnect(): could not find a connection path between the components!");
		if(tile->value != TILE_EMPTY) error("stageConnect(): not an empty tile after pre connection stage ~~~~ ");
		
		//now connect them
		
		while(tile->value == TILE_EMPTY)
		{
			tile->value = 1;
			i2 = tile->prev[0];
			j2 = tile->prev[1];
			tile = stageGetTile(stage,i2,j2);
		}
		iterationCount++;
	}
	if(iterationCount >= 50000) error("levelgen: stageConnect(): iterationCount >= 50000!");
	
	int i,j,k,i2,j2,i3,j3,count,curIter,maxIter,borderValues[4];
	int sourceX,sourceY,targetX,targetY,sizeX,sizeY;
	TILE *tile, *tile2, *borderTiles[4];
	// widen narrow paths
	for(i = 2; i < stage->size[0]-1; i++)
	{
		for(j = 2; j < stage->size[1]-1; j++)
		{
			tile = stageGetTile(stage,i,j);
			if(tile->value)
			{
				count = 0;
				for(k = 0; k < 4; k++)
				{
					i2 = i+levelgenOffset2D[k*2+0];
					j2 = j+levelgenOffset2D[k*2+1];
					tile2 = stageGetTile(stage,i2,j2);
					borderTiles[k] = tile2;
					borderValues[k] = !tile2->value;
					if(tile2->value == TILE_EMPTY) count++;
				}
				if(count == 2)
				{
					if(borderValues[0]*borderValues[2] || borderValues[1]*borderValues[3])
					{
						if(borderValues[0]*borderValues[2]) borderTiles[0]->value = 2;
						else borderTiles[3]->value = 2;
					}
				}
			}
		}		
	}
	
	
}

void stageDoFlood(STAGE* stage, int startX, int startY, int floodId, int floodMax, int mode)
{
	int i,j,k,i2,j2,i3,j3,iterationCount = 0,found;
	TILE* tile, *tile2;
	LEVELGENSTACK* workingStack;
	
	workingStack = stage->workingStack;
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			tile->flood[floodId] = FLOOD_VALUE_MAX;
		}		
	}
	if(floodId == FLOOD_EXIT) stage->floodExitMax = 0;
	tile = stageGetTile(stage,startX,startY);
	tile->flood[floodId] = 0;
	(workingStack->values)[0] = startX;
	(workingStack->values)[1] = startY;
	workingStack->stackCurrent = 0;
	workingStack->stackMax = 1;
	while(workingStack->stackCurrent < workingStack->stackMax)
	{
		i2 = (workingStack->values)[workingStack->stackCurrent*2+0];
		j2 = (workingStack->values)[workingStack->stackCurrent*2+1];
		tile = stageGetTile(stage,i2,j2);
		if(tile->flood[floodId] >= floodMax) break;
		for(k = 0; k < 4; k++)
		{
			i3 = i2+levelgenOffset2D[k*2+0];
			j3 = j2+levelgenOffset2D[k*2+1];
			if(i3 > 0 && i3 < stage->size[0]-1 && j3 > 0 && j3 < stage->size[1]-1)
			{
				tile2 = stageGetTile(stage,i3,j3);
				if(tile2->value != TILE_EMPTY && tile2->flood[floodId] == FLOOD_VALUE_MAX)
				{
					tile2->flood[floodId] = tile->flood[floodId]+1;
	if(floodId == FLOOD_EXIT) stage->floodExitMax = maxv(stage->floodExitMax,tile2->flood[floodId]);
					if(workingStack->stackMax >= workingStack->stackSize) error("stageDoFlood(): stack overflow! Wow!");
					(workingStack->values)[workingStack->stackMax*2+0] = i3;
					(workingStack->values)[workingStack->stackMax*2+1] = j3;
					workingStack->stackMax++;
				}
			}
		}
		workingStack->stackCurrent++;
	}
}

void stageAddExitAndEntrance(STAGE* stage)
{
	int i,j,k,i2,j2,i3,j3,i4,j4,iterationCount = 0,shiftX,shiftY,ok,found;
	TILE* tile, *tile2, *tile3;
	LEVELGENSTACK* workingStack;
	
	workingStack = stage->workingStack;
	// add exit
	found = 0;
	shiftX = random(stage->size[0]);
	shiftY = random(stage->size[1]);
	for(k = 0; k < 2; k++) // second iteration in case first round could not find an exit position
	{
		for(i = 0; i < stage->size[0]; i++)
		{
			for(j = 0; j < stage->size[1]; j++)
			{
				i2 = (i+shiftX)%stage->size[0];
				j2 = (j+shiftY)%stage->size[1];
				if(i2 > 1 && i2 < stage->size[0]-2 && j2 > 1 && j2 < stage->size[1]-2)
				{
					tile = stageGetTile(stage,i2,j2);
					if(tile->value != TILE_EMPTY)
					{
						ok = 1;
						if(!k) // prefer an exit position in a walkable 3x3 area
						{
							for(i3 = -1; i3 <= 1; i3++)
							{
								for(j3 = -1; j3 <= 1; j3++)
								{
									if(i3 || j3)
									{
										i4 = i2+i3;
										j4 = j2+j3;
										tile = stageGetTile(stage,i4,j4);
										if(tile->value == TILE_EMPTY)
										{
											ok = 0;
											i3 = 2;
											break;
										}
									}
								}
							}
						}
						if(ok)
						{
							stage->exitPos[0] = i2;
							stage->exitPos[1] = j2;
							i = stage->size[0];
							found = 1;
							tile = stageGetTile(stage,i2,j2);
							tile->flags |= TILE_FLAG_EXIT;
							break;
						}
					}
				}
			}		
		}
		if(found) break;
	}
	if(!found) error("stageAddExitAndEntrance(): could not find a position to place an exit!");
	
	stageDoFlood(stage,stage->exitPos[0],stage->exitPos[1],FLOOD_EXIT,FLOOD_VALUE_MAX,0);
	if(workingStack->stackMax < 30) error("stageAddExitAndEntrance(): strange workingStack->stackMax after FLOOD_EXIT!");
	
	workingStack->stackCurrent = workingStack->stackMax*(0.9+random(0.1))-1; //-random(10);
	i2 = (workingStack->values)[workingStack->stackCurrent*2+0];
	j2 = (workingStack->values)[workingStack->stackCurrent*2+1];
	tile = stageGetTile(stage,i2,j2);
	tile->flags |= TILE_FLAG_ENTRANCE;
	stage->entrancePos[0] = i2;
	stage->entrancePos[1] = j2;
	
	stage->numWalkableTiles = 0;
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage,i,j);
			if(tile->value != TILE_EMPTY) stage->numWalkableTiles++;
		}		
	}
}

int stageCreateEnemyIsPositionValid(STAGE* stage, int i2, int j2)
{
	TILE* tile;
	
	tile = stageGetTile(stage,i2,j2);
	if(!tile) error("stageCreateEnemyIsPositionValid: no tile !!!!!!");
	if(tile->value == TILE_EMPTY
	|| (tile->flags & TILE_FLAG_ENEMYSPAWN)
	|| (abs(i2-stage->exitPos[0]) <= 1 && abs(j2-stage->exitPos[1]) <= 1)
	|| (abs(i2-stage->entrancePos[0]) <= 3 && abs(j2-stage->entrancePos[1]) <= 3)) return 0;
	return 1;
}

void stageCreateEnemyData(STAGE* stage)
{
	int i,j,i2,j2,i3,j3,k,enemyCur,found,enemyType;
	var maxDiff;
	TILE* tile, *tile2;
	LEVELGENSTACK* workingStack;
	
	if(stage->enemyPositions) error("stageCreateEnemyData(): stageNew->enemyPositions != NULL!");
	//if(!stage->numWalkableTiles) error("stageCreateEnemyData(): stage->numWalkableTiles == 0!");
	stage->numEnemies = minv(stage->numWalkableTiles*0.333, 15 + 5 * stage->level+random(5));
	stage->enemyPositions = (VECTOR*)sys_malloc(sizeof(VECTOR)*stage->numEnemies);
	stage->enemyData = (int*)sys_malloc(sizeof(int)*stage->numEnemies*2);
	
	workingStack = stage->workingStack;		
	stageDoFlood(stage,stage->exitPos[0],stage->exitPos[1],FLOOD_EXIT,FLOOD_VALUE_MAX,0);
	maxDiff = workingStack->stackMax/(float)(stage->numEnemies+2); // stage->numWalkableTiles
	for(enemyCur = 0; enemyCur < stage->numEnemies; enemyCur++)
	{
		workingStack->stackCurrent = (enemyCur+1)/(float)(stage->numEnemies+2)*workingStack->stackMax+random(maxDiff); //-random(10);
		i2 = (workingStack->values)[workingStack->stackCurrent*2+0];
		j2 = (workingStack->values)[workingStack->stackCurrent*2+1];
		if(!stageCreateEnemyIsPositionValid(stage,i2,j2))
		{
			found = 0;
			for(k = 1; k < minv(stage->size[0],stage->size[1]); k++)
			{
				for(i = -k; i <= k; i++)
				{
					for(j = -k; j <= k; j++)
					{
						i3 = i2+i;
						j3 = j2+j;
						if(i3 > 0 && i3 < stage->size[0]-1 && j3 > 0 && j3 < stage->size[1]-1)
						{
							//printf("test pos: (%d,%d) (dist %d, %d)",i2,j2,(int)abs(i2-stage->entrancePos[0]),(int)abs(j2-stage->entrancePos[1]));
							if(stageCreateEnemyIsPositionValid(stage,i3,j3))
							{
								i2 = i3;
								j2 = j3;
								tile = stageGetTile(stage,i2,j2);
								if(tile->flags & TILE_FLAG_ENTRANCE) error("AKJSDASDAKDKASD");
								i = k+1;
								k = maxv(stage->size[0],stage->size[1]);
								found = 1;
								break;
							}
						}	
					}
				}
			}
			if(!found) error("stageCreateEnemyData(): SUPERERROR!");
		}
		
		stageGetPosFromIndices(stage, &(stage->enemyPositions)[enemyCur], i2, j2);
		//cprintf4("\n%d): (%d,%d,%d)",enemyCur,(int)(stage->enemyData)[enemyCur].x,(int)(stage->enemyData)[enemyCur].y,(int)(stage->enemyData)[enemyCur].z);
		tile = stageGetTile(stage,i2,j2);
		enemyType = 0+random(5+(enemyCur < stage->numEnemies*0.5)*1.9);
		if(enemyType == 0) tile->flags |= TILE_FLAG_TRAP_SPIKES;
		if(enemyType == 1) tile->flags |= TILE_FLAG_TRAP_HOLE;
		if(enemyType == 2) tile->flags |= TILE_FLAG_TRAP_TURRET;
		if(enemyType == 3) tile->flags |= TILE_FLAG_TRAP_BAT;
		if(enemyType == 4) tile->flags |= TILE_FLAG_TRAP_SPHERE;
		if(enemyType >= 5) tile->flags |= TILE_FLAG_TRAP_SPUTNIK;
		tile->flags |= TILE_FLAG_ENEMYSPAWN;
		(stage->enemyData)[enemyCur*2+0] = i2;
		(stage->enemyData)[enemyCur*2+1] = j2;
	}	
}

int stageGetNumEnemies(STAGE* stage)
{
	return stage->numEnemies;
}

VECTOR* stageGetEnemyPosition(STAGE* stage, int idx)
{
	if(idx < 0 || idx >= stage->numEnemies) return NULL;
	return &(stage->enemyPositions)[idx];
}

void stageSetEnemyType(STAGE* stage, int idx, int flagType)
{
	TILE* tile;
	
	if(idx < 0 || idx >= stage->numEnemies) return;
	tile = stageGetTile(stage,(stage->enemyData)[idx*2+0],(stage->enemyData)[idx*2+0]);
	tile->flags |= flagType;
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
				//vec_fill(vColor,128);
				if(tile->flood[FLOOD_EXIT]) vec_lerp(vColor,COLOR_GREEN,COLOR_RED,minv(tile->flood[FLOOD_EXIT]/48.0,1));
				else vec_fill(vColor,128);
				if(tile->value == 2) vec_set(vColor,vector(255,0,255));
			}
			else vec_fill(vColor,16);
			draw_quad(NULL, vector(posX+i*tileSize,posY+j*tileSize,0), NULL, vector(tileSize,tileSize,0), NULL, vColor, 100, 0);
			if(tile->flags & TILE_FLAG_ENTRANCE)
			{
				draw_text("S",posX+i*tileSize+4,posY+j*tileSize-1,COLOR_BLACK);
				draw_text("S",posX+i*tileSize+3,posY+j*tileSize-2,COLOR_WHITE);
			}
			if(tile->flags & TILE_FLAG_ENEMYSPAWN)
			{
				draw_text("E",posX+i*tileSize+4,posY+j*tileSize-1,COLOR_BLACK);
				draw_text("E",posX+i*tileSize+3,posY+j*tileSize-2,COLOR_WHITE);
			}
			if(tile->flood[FLOOD_PLAYER] < 9999)
			{
				draw_text(str_printf(NULL,"%2d",tile->flood[FLOOD_PLAYER]),posX+i*tileSize+0,posY+j*tileSize-2,COLOR_WHITE);
			}
		}		
	}
}

// vresult can be NULL, wanted near 1 -> position near entrance
VECTOR* stageGetQuestPosition(STAGE* stage, VECTOR* vresult, float wanted, float tolerance)
{
	int i,j,k,i2,j2,shiftX,shiftY;
	float fac;
	TILE* tile;
	
	if(!stage->floodExitMax) error("stageGetQuestPosition(): !stage->floodExitMax");
	shiftX = random(stage->size[0]);
	shiftY = random(stage->size[1]);
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			i2 = (i+shiftX)%stage->size[0];
			j2 = (j+shiftY)%stage->size[1];
			if(i2 > 1 && i2 < stage->size[0]-2 && j2 > 1 && j2 < stage->size[1]-2)
			{
				tile = stageGetTile(stage,i2,j2);
				if(tile->flood[FLOOD_EXIT] < FLOOD_VALUE_MAX)
				{
					fac = tile->flood[FLOOD_EXIT]/(float)stage->floodExitMax;
					if(abs(fac-wanted) < tolerance)
					{
						VECTOR* pv = stageGetPosFromIndices(stage, NULL, i2, j2);
						if(vresult && pv) vec_set(vresult,pv);
						return pv;
					}
				}
			}
		}
	}

	return NULL;
}





#include "levelcreator.c"

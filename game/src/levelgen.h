
int levelgenOffset2D[8] = { -1,0, 0,1, 1,0, 0,-1 };

#define TILE_EMPTY 0

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
	memset(stageNew->tiles,0,sizeX*sizeY);
	stageNew->workingStack = levelgenStackCreate(sizeX*sizeY*2);
	
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
			if(!i || i >= stage->size[0]-1 || !j || j >= stage->size[1]-1) tile->value = TILE_EMPTY;
			else
			{
				tile->value = maxv(random(2)-0.0,0);
			}
		}		
	}
	
	// remove noise (empty 1x1 areas)
	for(i = 1; i < stage->size[0]-1; i++)
	{
		for(j = 1; j < stage->size[1]-1; j++)
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
					if(tile2->value == TILE_EMPTY) count++;
				}
				if(count == 4) tile->value = TILE_EMPTY;
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
			tile->value = 2;
			i2 = tile->prev[0];
			j2 = tile->prev[1];
			tile = stageGetTile(stage,i2,j2);
		}
		
	}
	if(iterationCount >= 50000) error("levelgen: stageConnect(): iterationCount >= 50000!");
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
				if(tile->value == 2) vec_set(vColor,vector(0,0,255));
			}
			else vec_fill(vColor,16);
			draw_quad(NULL, vector(posX+i*tileSize,posY+j*tileSize,0), NULL, vector(tileSize,tileSize,0), NULL, vColor, 100, 0);
		}		
	}
}


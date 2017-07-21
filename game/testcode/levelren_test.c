///////////////////////////////

#define PRAGMA_PATH "../src/"
#define PRAGMA_PATH "../models/"

#include <acknex.h>
#include <default.c>
#include "datatypes.h"
#include "levelgen.h"
#include "levelcreator.c"
///////////////////////////////



void main()
{
	fps_max = 60;
	level_load(NULL);
	
	stageRenderInit();
	
	STAGE* stage = stageCreate(32,32,137.1);
	stageFill(stage);
	
	ENTITY * ent = stageRenderEntity(stage);
	
	
	
	while(1)
	{
		stageDraw(stage, screen_size.x - 4 * stage->size[0], 0, 4);
		
		wait(1);
	}
}



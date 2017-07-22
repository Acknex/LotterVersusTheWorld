///////////////////////////////

#define PRAGMA_PATH "../src/"
#define PRAGMA_PATH "../models/"
#define PRAGMA_PATH "../shaders/"
#define PRAGMA_PATH "../graphics/"

#include <acknex.h>
#include <default.c>
#include <mtlFx.c>
#include "datatypes.h"
#include "levelgen.h"
#include "levelcreator.c"
///////////////////////////////

void main()
{
	fps_max = 60;
	level_load(NULL);
	
	STAGE* stage = stageCreate(32,32,137.1);
	stageFill(stage);
	
	stage_load(stage);
	
	
	while(1)
	{
		stageDraw(stage, screen_size.x - 4 * stage->size[0], 0, 4);
		wait(1);
	}
}



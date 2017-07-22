///////////////////////////////
#include <acknex.h>
#include <default.c>
#include "src\\datatypes.h"
#include "src\\levelgen.h"
///////////////////////////////



void main()
{
	fps_max = 60;
	level_load(NULL);
	
	STAGE* stage = stageCreate(32,32,8172.607,0); //137.1);
	stageFill(stage);
	stageConnect(stage);
	stageAddExitAndEntrance(stage);
	stageCreateEnemyData(stage);
	
	while(1)
	{
		DEBUG_VAR(stage->seed,20);
		stageDraw(stage, 64, 64, 16);
		
		wait(1);
	}
}



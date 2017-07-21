///////////////////////////////
#include <acknex.h>
#include <default.c>
#include "levelgen.h"
///////////////////////////////



void main()
{
	fps_max = 60;
	level_load(NULL);
	
	STAGE* stage = stageCreate(32,32,137.1);
	stageFill(stage);
	
	while(1)
	{
		
		stageDraw(stage, 64, 16, 16);
		
		wait(1);
	}
}



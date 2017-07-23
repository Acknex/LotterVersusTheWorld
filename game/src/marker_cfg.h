#ifndef MARKER_CFG_H
#define MARKER_CFG_H

#include <acknex.h>
var vMarkerCount = 3;

#include "entity_defs.h"

TEXT* txtMarkers = 
{
	strings = 50;
		
	string(	
	/*TypeDefault 0*/          ".default\n 0x0815",
	/*TypePlayerProjectile 1*/ ".bugfix\n 0xDEC0DED",
	/*TypeEnemyProjectile 2*/  ".evilbullet\n 0xDEADC0DE",
	/*TypePlayer 3*/           ".lotti\n 0xB00B5",
	/*TypeWall 4*/             "",
	/*TypeDestroyed 5*/        ".destroyed\n 0xDEAD",
	/*TypeTurret 6*/           ".turret\n 0xBAFFBAFF",
	/*TypeTurret2 7*/          ".turret 2.0\n 0xBADA55",
	/*TypeSpikes 8*/           ".spikes\n 0xBEEFED",
	/*TypeHole 9*/             ".hole\n 0xC0FFEE",
	/*TypeEnemy 10*/           ".boring standard enemy\n 0x4711",
	/*TypeEnemy 11*/           ".retrovirus\n 0xBADD00D",
	/*TypeEnemy 12*/           ".sphere\n 0xDEADBA11"
	/*TypeQuestmaster 13*/     ".questmaster\n 0x1337"
	/*TypeQuestItem 14*/       ".questitem\n 0x1337"
	);
}

#endif

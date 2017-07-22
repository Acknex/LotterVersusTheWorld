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
	/*TypeTurret 62*/          ".turret 2.0\n 0xBADA55",
	".spikes",
	".hole",
	".enemy"
	);
}

#endif

#define TypeDefault 0
#define TypePlayerProjectile 1
#define TypeEnemyProjectile 2
#define TypePlayer 3
#define TypeWall 4
#define TypeDestroyed 5
#define TypeTurret 6
#define TypeTurret2 7

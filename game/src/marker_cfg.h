#ifndef MARKER_CFG_H
#define MARKER_CFG_H

#include <acknex.h>
var vMarkerCount = 3;

#include "entity_defs.h"

TEXT* txtMarkers = 
{
	strings = 50;
		
	string(	
	".default\n 0x0815",
	"",
	".evilbullet\n 0xDEADC0DE",
	".lotti\n 0xB00B5",
	".bugfix\n 0xDEC0DED",
	".destroyed\n 0xDEAD",
	".turret\n 0xBAFFBAFF"
	);
}

#endif
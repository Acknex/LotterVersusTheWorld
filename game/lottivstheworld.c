#include <acknex.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"

#include "player.h"
#include "camera.h"
#include "materials.h"
#include "level.h"

#define DEBUG

#ifdef DEBUG
	#include <default.c>
#endif

void main() 
{
	d3d_antialias = 9;
	on_resize = INIT_resize;	
	INIT_global();
	wait(1);
	LEVEL_start();
	LEVEL_loop();
}
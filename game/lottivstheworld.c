#include <acknex.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"
#define PRAGMA_PATH "graphics"

#include "player.h"
#include "camera.h"
#include "materials.h"
#include "init.h"
#include "weapon.h"
#include "items.h"
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
	INIT_levelStart();
	INIT_levelLoop();
}
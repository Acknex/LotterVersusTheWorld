#include <acknex.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"

#include "player.h"
#include "camera.h"
#include "bloom.h"
#include "level.h"

#define DEBUG

#ifdef DEBUG
	#include <default.c>
#endif

void main() 
{
	d3d_antialias = 9;
	wait(1);
	create_camera();
	LEVEL_start();
	LEVEL_loop();
}
#include <acknex.h>
#include <windows.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"
#define PRAGMA_PATH "graphics"

#include "mouse.h"
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

void quitGame()
{
	//do not add anything here - use INIT_exit in init.c!
	sys_exit("");
}

void main() 
{
	max_entities = 20000;
	d3d_antialias = 0;
	video_mode = 12;
	on_resize = INIT_resize;
	INIT_start();
	wait(1);
	INIT_levelStart();
	INIT_levelLoop();
	
	on_exit = INIT_exit;
	on_close = quitGame;

	//temp - until menu is introduced
	on_esc = quitGame;
}


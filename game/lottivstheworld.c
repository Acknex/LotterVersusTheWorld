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

void quitGame()
{
	//do not add anything here - use INIT_exit in init.c!
	sys_exit("");
}

void main() 
{
	d3d_antialias = 9;
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


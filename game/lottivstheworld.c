#include <acknex.h>
#include <windows.h>

#include "config.h"

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"
#define PRAGMA_PATH "graphics"

#include "cheats.h"
#include "mouse.h"
#include "player.h"
#include "camera.h"
#include "materials.h"
#include "init.h"
#include "weapon.h"
#include "items.h"
#include "hud.h"
#include "splash.h"


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
	
#ifdef WINDOWMODE
	video_mode = 10;
#else
	video_window(vector(0, 0, 0), nullvector, 1, "Lotter vs. the World");
	video_set(sys_metrics(0), sys_metrics(1), 0, 2);
#endif

	credits_init();
	
	on_resize = INIT_resize;
	INIT_start();
	wait(1);
	INIT_globalLoop();
#ifdef FASTLOAD
#ifdef FASTCREDITS
	credits_start();
#else
	INIT_levelStart();
	INIT_levelLoop();
#endif
#else
	SPLASH__init();
#endif

	MARKER_toggle();
	
	on_exit = INIT_exit;
	on_close = quitGame;

	//temp - until menu is introduced
	on_esc = quitGame;
}


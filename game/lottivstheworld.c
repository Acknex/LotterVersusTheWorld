#include <acknex.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"

#include "player.h"
#include "camera.h"

//#define DEBUG
#ifdef DEBUG
	#include <default.c>
#endif

void main() {
	d3d_antialias = 9;
	wait(1);
	create_camera();
	level_load("level\\test_level_small.wmb");
	camera.z = 600;
	//camera.tilt = 315;
	camera.tilt = 270;
	player_init();
	while(1) {
		player_move();
		camera.x = player.x;
		camera.y = player.y;
		wait(1);
	}
}
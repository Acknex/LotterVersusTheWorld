
#include "init.h"

action teleporter_out()
{
	ent_create("Teleport.mdl", my.x, NULL);
	set(me, PASSABLE);
	set(me, FLAG2);
	while(!player) wait(1);
	
	proc_mode = PROC_LATE;
	
	var portloader = 0;
	while(me)
	{
		var dist = vec_dist(vector(player.x, player.y, 0), vector(me.x, me.y, 0));
		if(dist < 50) // ist höhenabhängig!
		{
			player.near_teleport = 1;
			player.z += time_step;
			player.pan += 0.1 * portloader * time_step;
			portloader += 0.5 * time_step;
			if(portloader >= 75) {
				player.pan += 0.1 * portloader * time_step;
				player.pause_control = 1;
				var alpha = (portloader - 75) * 4;
				draw_quad(
					NULL,
					NULL, NULL,
					screen_size, NULL,
					COLOR_BLACK, alpha, 0);
			}
			if(portloader >= 100) {
				INIT_levelEnd();
				return;
			}
		}
		else 
		{
			player.near_teleport = 0;
			portloader = maxv(0, portloader - 10 * time_step);
		}
		DEBUG_VAR(dist, 512);
		DEBUG_VAR(portloader, 528);
		my->skill41 = floatv(0.001 + 0.001 * portloader);
		wait(1);
	}
}
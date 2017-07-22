
#include "init.h"

action teleporter_out()
{
	set(me, PASSABLE);
	set(me, FLAG2);
	while(!player) wait(1);
	
	var portloader = 0;
	while(me)
	{
		var dist = vec_dist(player.x, me.x);
		if(dist < 170) // ist höhenabhängig!
		{
			portloader += time_step;
			if(portloader >= 100) {
				INIT_levelEnd();
				return;
			}
		}
		else 
		{
			portloader = maxv(0, portloader - 10 * time_step);
		}
		DEBUG_VAR(dist, 512);
		DEBUG_VAR(portloader, 528);
		my->skill41 = floatv(0.001 + 0.001 * portloader);
		wait(1);
	}
}
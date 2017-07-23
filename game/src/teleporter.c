
#include "init.h"

ENTITY * teleporterEntity = NULL;

void teleporter_enable()
{
	reset(teleporterEntity, INVISIBLE);
}

void teleporter_disable()
{
	set(teleporterEntity, INVISIBLE);
}

function teleport_effect(PARTICLE * p)
{
	p->flags = BEAM | BRIGHT | MOVE;
	vec_set(p->vel_x, vector(0, 0, 2 + random(5)));
	vec_set(p->blue, COLOR_WHITE);
	vec_add(p->x, vec_rotate(vector(60,0,0), vector(random(360), 0, 0)));
	p->lifespan = 100;
	p->event = NULL;
}

action teleporter_out()
{
	teleporterEntity = me;
	you = ent_create("Teleport.mdl", my.x, NULL);
	set(me,  PASSABLE | FLAG2 | INVISIBLE);
	set(you, PASSABLE | FLAG2);
	while(!player) wait(1);
	
	proc_mode = PROC_LATE;
	
	var portloader = 0;
	while(1)
	{
		var dist = vec_dist(vector(player.x, player.y, 0), vector(me.x, me.y, 0));
		if(!is(me, INVISIBLE) && dist < 70) // ist höhenabhängig!
		{
			if(dist > 5)
			{
				var force = 10 * pow(dist / 50, 1);
				
				VECTOR dir;
				vec_diff(dir, my.x, player.x);
				dir.z = 0;
				vec_normalize(dir, force * time_step);
			
				vec_add(player.x, dir);
			}
			
			player.near_teleport = 1;
			player.z += time_step;
			player.pan += 0.1 * portloader * time_step;
			portloader += time_step;
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
			effect(teleport_effect, 3 + time_frame, my.x, vector(0,0,0));
		}
		else 
		{
			player.near_teleport = 0;
			portloader = maxv(0, portloader - 10 * time_step);
		}
		my->skill41 = floatv(0.001 + 0.001 * portloader);
		wait(1);
	} 
}
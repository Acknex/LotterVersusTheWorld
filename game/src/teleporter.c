
#include "init.h"

ENTITY * teleporterEntity = NULL;
ENTITY * teleporterSocket = NULL;
SOUND * sndTeleport = "teleport_loop.wav";

void teleporter_enable()
{
	reset(teleporterEntity, INVISIBLE);
	teleporterSocket->type = TypeTeleporterEnabled;
}

void teleporter_disable()
{
	set(teleporterEntity, INVISIBLE);
	teleporterSocket->type = TypeTeleporterDisabled;
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

STRING * teleporter_message = "#128";

action teleporter_out()
{
	teleporterEntity = me;
	teleporterSocket = ent_create("Teleport.mdl", my.x, NULL);
	teleporterSocket.type = TypeTeleporterDisabled;
	teleporterSocket.material = ObjectMaterial;
	teleporterSocket.skill41 = floatv(29);
	set(me,  PASSABLE | FLAG2 | INVISIBLE);
	my.material = TeleporterEffectMaterial;
	set(teleporterSocket, PASSABLE | FLAG2);
	while(!player) wait(1);
	
	proc_mode = PROC_LATE;
	
	var sndHandle = 0;
	var sndVolume = 0;
	var portloader = 0;
	while(1)
	{
		if(hex_isDead) {
			str_cpy(teleporter_message, "GUARDIAN:\n  Shut down\n");
		} else {
			str_cpy(teleporter_message, "GUARDIAN:\n  Active\n");
		}
		
		if(is(me, INVISIBLE)) {
			str_cat(teleporter_message, "TELEPORT:\n  Disabled");
		} else {
			str_cat(teleporter_message, "TELEPORT:\n  Enabled");
		}
		
		MARKER_setText(teleporterSocket, teleporter_message);
		
		var dist = vec_dist(vector(player.x, player.y, 0), vector(me.x, me.y, 0));
		if(!is(me, INVISIBLE) && dist < 70) // ist höhenabhängig!
		{
			if(sndHandle == 0) 
			{
				sndHandle = snd_loop(sndTeleport, 0, 0);
			}
			else if(sndVolume < 100)
			{
				sndVolume = minv(100, sndVolume + 5*time_step);
				snd_tune(sndHandle, sndVolume, 0, 0);
			}
			
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
				INIT_levelEnd(0);
				snd_stop(sndHandle);
				return;
			}
			effect(teleport_effect, 3 + time_frame, my.x, vector(0,0,0));
		}
		else 
		{
			if(sndHandle != 0)
			{
				if(sndVolume > 0)
				{
					sndVolume = maxv(0, sndVolume - 5*time_step);
					snd_tune(sndHandle, sndVolume, 0, 0);
				}
				else
				{
					snd_stop(sndHandle);
					sndHandle = 0;
				}
			}
			player.near_teleport = 0;
			portloader = maxv(0, portloader - 10 * time_step);
		}
		my->skill41 = floatv(0.001 + 0.001 * portloader);
		wait(1);
	} 
}
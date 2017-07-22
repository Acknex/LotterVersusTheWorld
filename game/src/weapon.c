void weapon_startup()
{
	while(1)
	{
		if((player != NULL) && mouse_left) 
		{
			player.skill44 = 20;
			shoot();
		}
		wait(1);		
	}
}

#include "camera.h"
void ricochet_effect()
{
	var t = 0;
	my.flags |= LIGHT | PASSABLE | TRANSLUCENT;
	my.red = 255;
	my.green = 128;
	my.blue = 255;
	if(my.pan == 0)
		my.pan = 0.01;
	if(my.tilt == 0)
		my.tilt = 0.01;
	
	vec_scale(my.scale_x, 0.5);
	while(t < 0.3) {
		var f = t / 0.3;
		var scale = 0.01+f*1;
		vec_set(my.scale_x, vector(scale,scale,scale));
		my.alpha = (1-f)*100;
		t += time_step/16;
		DEBUG_VAR(t,150);
		wait(1);
	}
	ptr_remove(my);
}

void projectile()
{
	var t = 0;
	
	BMAP *blub = ent_getskin(me, 1);
	txt_fragment->target_map = blub;
	
	vec_set(my.pan, vector(player.pan + weapon_angle_correction, 0, 0));
	
	my.flags |= (FLAG2 | PASSABLE);
	
	VECTOR dir;
	VECTOR offset;
	VECTOR to;
	vec_for_angle(dir, vector(player.pan + weapon_angle_correction, 0, 0));
	vec_set(offset, dir);
	vec_scale(dir, weapon_speed);
	
	vec_scale(offset, weapon_projectile_spawn_offset);
	vec_add(my.x, offset);
	
	my.tilt = 90;
	my.pan += 90;
	
	vec_scale(my.scale_x, 0.6);
	c_setminmax(me);
	
	my.skill50 = 0; // How many time a projectile has bounced already
	
	var dist = 2;
	while(1)
	{
		t += time_step / 16; //Dead after timer if projectile is shot into the wild
		
		vec_add(my.x, vector(dir.x * time_step, dir.y * time_step, dir.z * time_step) );
		
		my.flags |= LIGHT;
		my.red = 255;
		my.green = 128;
		my.blue = 255;
		my.lightrange = 16;
		
		vec_set(to, dir);
		vec_normalize(to, 1);
		vec_set(offset, to);
		vec_scale(to, 72);
		vec_scale(offset, 64);
		vec_add(to, my.x);
		vec_add(offset, my.x);
		dist = c_trace(offset, to, IGNORE_ME | IGNORE_PASSABLE | ACTIVATE_SHOOT);
		
		draw_line3d(to, NULL, 100);
		draw_line3d(offset, COLOR_GREEN, 100);
		draw_line3d(to, COLOR_GREEN, 100);
		
		if(you == player)	{ break; }
		
		if((dist != 0 || t > 5) && player.skill44 == 0 ) 
		{ 
			VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
			vec_normalize(v, 1);
			vec_add(v.x, hit.x);
			ENTITY* ricochet = ent_create("ricochet.tga", v, ricochet_effect);
			vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));
			//vec_add(ricochet->x, hit.nx);
			break; 
		}
		else if( dist != 0 && player.skill44 > 0 && my.skill50 < player.skill44 )
		{
			VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
			vec_normalize(v, 1);
			vec_add(v.x, hit.x);
			ENTITY* ricochet = ent_create("ricochet.tga", v, ricochet_effect);
			vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));
			vec_set(dir, bounce);
			vec_scale(dir, weapon_speed);
			vec_to_angle(my.pan, dir);
			my.tilt = 90;
			my.pan += 90;
			my.skill50 += 1;
		} 
		else if (dist != 0 || t > 5)
		{
			break;
		}
		wait(1);
	}
	ptr_remove(me);
}

void cooldown()
{
	if(proc_status(cooldown) == 0)
	{
		while(player.skill43 < weapon_cooldown_time && mouse_left)
		{
			player.skill43 += time_step / 16;
			wait(1);
		}
		player.skill43 = 0;
	}
}

void shoot()
{
	VECTOR to;
	
	if(player.skill43 == 0)
	{
		ent_create("billboard.tga", player.x, projectile);
		cooldown();
	}
	
	//vec_for_angle(to, player.pan);
	//vec_scale(to, 1000);
	//vec_add(to, player.x);
}
void weapon_startup()
{
	while(1)
	{
		if((player != NULL) && mouse_left) 
		{
			player.skill44 = 2;
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
	
	vec_scale(my.scale_x, 0.5);
	wait(1);
	if(my.pan == 0)
		my.pan = 0.01;
	if(my.tilt == 0)
		my.tilt = 0.01;
	my.roll = random(360);
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
	txt_fragment->flags |= SHOW;
	
	vec_set(my.pan, vector(player.pan + weapon_angle_correction, 0, 0));
	
	my.flags |= (PASSABLE);
	my.type = TypePlayerProjectile;
	
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
	
	vec_scale(my.scale_x, weapon_projectile_scale);
	c_setminmax(me);
	
	my.skill50 = 0; // How many time a projectile has bounced already
	
	var dist = 2;
	while(1)
	{
		t += time_step / 16; //Dead after timer if projectile is shot into the wild
		
		my.flags |= LIGHT;
		my.red = 255;
		my.green = 128;
		my.blue = 255;
		
		vec_set(to, dir);
		vec_normalize(to, 1);
		vec_set(offset, to);
		vec_scale(to, 16 * weapon_projectile_scale);
		vec_add(to, my.x);
		//vec_scale(offset, -16);
		//vec_add(offset, my.x);
		dist = c_trace(my.x, to, IGNORE_ME | IGNORE_PASSABLE | ACTIVATE_SHOOT);
		
		
		/*
		draw_line3d(to, NULL, 100);
		draw_line3d(offset, COLOR_GREEN, 100);
		draw_line3d(to, COLOR_GREEN, 100);
		*/
		if(you == player)	{ break; }
		
		if((dist != 0 || t > weapon_lifetime) && player.skill44 == 0 ) 
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
		else if (dist != 0 || t > weapon_lifetime)
		{
			break;
		}
		else
		{
			vec_add(my.x, vector(dir.x * time_step, dir.y * time_step, dir.z * time_step) );
		}
		wait(1);
	}
	ptr_remove(me);
}

void granate()
{
	VECTOR offset;
	VECTOR dir;
	VECTOR from;
	
	my.flags |= LIGHT | PASSABLE;
	my.red = 255;
	my.green = 128;
	my.blue = 255;
	
	VECTOR *mouse_from = vec_for_screen(vector(mouse_pos.x, mouse_pos.y, 1), get_camera());
	VECTOR *mouse_to = vec_for_screen(vector(mouse_pos.x, mouse_pos.y, 5000), get_camera());
	c_trace(mouse_from, mouse_to, USE_POLYGON);
	VECTOR aim;
	vec_set(aim, hit.x);
	
	vec_set(my.pan, vector(player.pan + weapon_angle_correction, 0, 0));
	
	vec_for_angle(dir, player.pan);
	
	
	
	var dist = vec_dist(aim, player.x);
	vec_set(from, player.x);
	
	var height = 256;
	
	var i; for(i=0; i<dist; i+=dist/100) 
	{
		
		var pz = -((4 * height) / pow(dist, 2)) * pow(i - dist / 2, 2) + height;
		
		vec_set(offset, dir);
		vec_scale(offset, i);
		vec_add(offset, from);
		draw_point3d(vector(offset.x, offset.y, pz), COLOR_RED, 100, 10);
		
		draw_line3d(player.x, NULL, 100);
		draw_line3d(player.x, COLOR_GREEN, 100);
		draw_line3d(aim, COLOR_GREEN, 100);
		
		draw_point3d(aim, COLOR_WHITE, 100, 20);
		wait(1);
	}
	
	ent_remove(me);
	
	/*while(1)
	{
		vec_add(active, offset);
		
		vec_set(my.x, active);
		
		var z = pow(vec_dist(active, player.x) - dist, 2);
		
		
		
		draw_point3d(vector(active.x, active.y, z), COLOR_RED, 100, 10);
		
		wait(1);
	}*/
	
}

void shotgun()
{
	
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
	
	player.weapon_type = 0;//1;
	
	if(player.skill43 == 0 && player.weapon_type == 0)
	{
		ent_create("billboard.tga", player.x, projectile);
		cooldown();
	}
	
	//Test für Granate
	
	if(player.weapon_type == 1)
	{
		
		if(mouse_left)
		{
			
		}
		
		ent_create("cube.mdl", player.x, granate);
		cooldown();
	}
}
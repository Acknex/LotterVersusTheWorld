void weapon_startup()
{
	while(1)
	{
		if((player != NULL) && mouse_left) 
		{
			player.skill44 = 2;
			player.group = 3;
			shoot(1);
		}
		else if((player != NULL) && mouse_right)
		{
			shoot(2);
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
	
	my.flags |= (PASSABLE | FLAG2);
	my.type = TypePlayerProjectile;
	my.damage = 1;
	
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
		c_ignore(3);
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

void granate_exp_event(PARTICLE *p)
{
	
}

void granate_explosion(PARTICLE *p)
{
	p->flags = MOVE | BRIGHT;
	p->size = 16;
	p->vel_x = 10 - random(20);
	p->vel_y = 10 - random(20);
	p->vel_z = 10;
	p->lifespan = 20;
	p->event = granate_exp_event;
}

void explosion(ENTITY *ent)
{
	effect(granate_explosion, 80,  ent.x, nullvector);
}

void granate()
{
	VECTOR vstart,temp, vTarget, midPos1, midPos2;
	
	my.group = 2;
	
	my.flags |= (PASSABLE |FLAG2);
	
	// Calculate grenate target
	vec_set(vTarget,mouse_dir3d);
	vec_scale(vTarget,1000);
	float t = -mouse_pos3d.z/vTarget.z;
	vec_scale(vTarget,t);
	vec_add(vTarget,mouse_pos3d);
	
	// Set start and two interpolated points
	vec_set(vstart,my.x);
	vec_lerp(midPos1,vstart,vTarget,0.4);
	midPos1.z = 200;
	vec_lerp(midPos2,vstart,vTarget,0.667);
	midPos2.z = 200;
	
	my.damage = 3;
	
	// Bezier interpolation
	while(my.skill1 < 16 && me)
	{
		my.skill1 = minv(my.skill1+time_step,16);
		float t = my.skill1/16.0;
		float tinv = 1-t;
		
		vec_set(my.x,vstart);
		vec_scale(my.x,tinv*tinv*tinv);
		
		vec_set(temp,midPos1);
		vec_scale(temp,tinv*tinv*t*3);
		vec_add(my.x,temp);
		
		vec_set(temp,midPos2);
		vec_scale(temp,tinv*t*t*3);
		vec_add(my.x,temp);
		
		vec_set(temp,vTarget);
		vec_scale(temp,t*t*t);
		
		c_move(me, nullvector, temp, IGNORE_ME | IGNORE_PASSABLE | IGNORE_PUSH);
		wait(1);
	}
	//explosion(me);
	c_scan(my.x, nullvector, vector(360, 0, 16), ACTIVATE_SHOOT);
	ent_remove(me);
}

void cooldown()
{
	if(proc_status(cooldown) == 0)
	{
		while(player.weapon_cooldown < weapon_cooldown_time && mouse_left)
		{
			player.weapon_cooldown += time_step / 16;
			wait(1);
		}
		player.weapon_cooldown = 0;
	}
}

void cooldown_granate()
{
	if(proc_status(cooldown_granate) == 0)
	{
		while(player.weapon_granade_cooldown < weapon_grenade_cooldown_time)
		{
			player.weapon_granade_cooldown += time_step / 16;
			wait(1);
		}
		player.weapon_granade_cooldown = 0;
	}
}

void shoot(int wp_type)
{
	if(player.weapon_cooldown == 0 && wp_type == 1)
	{
		ent_create("billboard.tga", player.x, projectile);
		cooldown();
	}
	
	//Test für Granate
	
	if(player.weapon_granade_cooldown == 0 && wp_type == 2)
	{
		ent_create("cube.mdl", player.x, granate);
		cooldown_granate();
	}
}
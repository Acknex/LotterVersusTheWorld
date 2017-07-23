SOUND* sndPlayerShot = "sounds\\player_shot.wav";
SOUND* sndGrenadeThrow = "sounds\\grenade_throw.wav";
SOUND* sndGrenadeExplode = "sounds\\grenade_explode.wav";

void weapon_startup()
{
	var shootingHandle = 0;
	while(1)
	{
		if((player != NULL) && mouse_left) 
		{
			if(player.health > 0)
			{
				if(shootingHandle == 0)
				{
					shootingHandle = snd_loop(sndPlayerShot, 50, 0);
				}
				player.weapon_bouncing = 2;
				player.group = 3;
				shoot(1);
			}
		}
		if(!mouse_left && shootingHandle != 0) 
		{
			snd_stop(shootingHandle);
			shootingHandle = 0;
		}
		if((player != NULL) && mouse_right)
		{
			if(player.health > 0)
			{
				shoot(2);
			}
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
	
	
	VECTOR vTarget;
	// Calculate grenate target
	vec_set(vTarget, mouse_dir3d);
	vec_scale(vTarget, 1000);
	float l = -mouse_pos3d.z / vTarget.z;
	vec_scale( vTarget, l);
	vec_add(vTarget, mouse_pos3d);
	
	vTarget.z += 0;
	
	
	
	my.group = 4;
	my.flags |= (FLAG2);
	my.type = TypePlayerProjectile;
	my.damage = 1;
	
	VECTOR dir;
	VECTOR offset;
	VECTOR to;
	
	vec_set(to, vTarget);
	vec_sub(vTarget, player.x);
	vec_to_angle(my.pan, vTarget);
	vec_normalize(vTarget, 1);
	vTarget.z += random(0.1);
	vec_scale(vTarget, weapon_speed * time_step);
	
	my.tilt = 90;
	my.pan += 90;
	
	vec_scale(my.scale_x, weapon_projectile_scale);
	
	my.skill21 = 0; // How many time a projectile has bounced already
	
	var dist = 2;
	while(1)
	{
		t += time_step / 16; //Dead after timer if projectile is shot into the wild
		
		my.flags |= LIGHT;
		my.red = 255;
		my.green = 128;
		my.blue = 255;
		
		vec_normalize(vTarget, 1);
		vec_scale(vTarget, weapon_speed * time_step);
		
		c_ignore(3,4,0);
		dist = c_move(me, nullvector, vTarget, ACTIVATE_SHOOT | USE_POLYGON);
		
		if(t >= weapon_lifetime)
		{
			break;
		}
		
		if((you || !dist) && (player.weapon_bouncing == 0 || my.skill21 >= player.weapon_bouncing))
		{
			VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
			vec_normalize(v, 1);
			vec_add(v.x, hit.x);
			ENTITY* ricochet = ent_create("ricochet.tga", v, ricochet_effect);
			vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));
			break; 
		}
		else if ( (you || !dist) && (my.skill21 < player.weapon_bouncing) )
		{
			if(you)
			{
				if(you.type == TypeDefault || you.type == TypeWall)
				{
					VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
					vec_normalize(v, 1);
					vec_add(v.x, hit.x);
					ENTITY* ricochet = ent_create("ricochet.tga", v, ricochet_effect);
					vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));
					vec_set(vTarget, bounce);
					vec_to_angle(my.pan, vTarget);
					my.tilt = 90;
					my.pan += 90;
					my.skill21 += 1;
					my.z += 10;
					t = weapon_lifetime - 0.5;
				} 
				else 
				{
					VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
					vec_normalize(v, 1);
					vec_add(v.x, hit.x);
					ENTITY* ricochet = ent_create("ricochet.tga", v, ricochet_effect);
					vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));
					break;
				}
			}
		}
		wait(1);
	}
	ptr_remove(me);
}

void granate()
{
	VECTOR vstart,temp, vTarget, midPos1, midPos2;
	
	my.group = 4;
	my.type = TypePlayerProjectile;
	my.damage = 3;
	my.flags |= (PASSABLE | FLAG2);
	
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
	
	
	// Bezier interpolation
	//var dist = 0;
	while(my.skill2 < 16 && me)
	{
		my.skill2 = minv(my.skill2+time_step,16);
		float t = my.skill2/16.0;
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
		
		c_move(me, nullvector, temp, IGNORE_ME | IGNORE_PASSABLE | IGNORE_PUSH );
		wait(1);
	}
	effect(p_granate_explode,200,my.x,nullvector);
	shake_cam(0.5);
	snd_play(sndGrenadeExplode, 100, 0);
	c_scan(my.x, nullvector, vector(360, 0, 300), SCAN_ENTS | IGNORE_ME);
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
	VECTOR spawn;
	vec_for_vertex(spawn, player, 2139);
	
	if(player.weapon_cooldown == 0 && wp_type == 1)
	{
		ent_create("billboard.tga", spawn, projectile);
		cooldown();
	}
	
	//Test für Granate
	
	if(player.weapon_granade_cooldown == 0 && wp_type == 2)
	{
		snd_play(sndGrenadeThrow, 100, 0);
		ent_create("sphere.mdl", spawn, granate);
		cooldown_granate();
	}
}
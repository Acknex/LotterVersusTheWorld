#ifndef WEAPON_H
#define WEAPON_H

	#include "Annoying.h"

	void projectile();
	void shoot(int wp_type);
	
	var weapon_speed = 60;
	var weapon_angle_correction = 0;
	var weapon_projectile_spawn_offset = 16;
	var weapon_cooldown_time = 0.12;
	var weapon_grenade_cooldown_time = 5;
	var weapon_lifetime = 5;
	var weapon_projectile_scale = 1.2;
	
	STRING *str_weapon_code = "::";
	
	BMAP *projectile_skin = "#32x32x32";
	
	FONT *monoid = "Monoid#32b";
	
	TEXT *txt_fragment =
	{
		font = monoid;
		pos_x = 16;
		pos_y = 16;
		flags = CENTER_X | CENTER_Y;
		strings = 1;
		string(str_weapon_code);
		red = 80;
		green = 255;
		blue = 80;
	}

	#include "weapon.c"

#endif
#ifndef WEAPON_H
#define WEAPON_H

	void shoot();
	void projectile();
	
	var weapon_speed = 32;
	var weapon_angle_correction = 0;
	var weapon_projectile_spawn_offset = 16;
	var weapon_cooldown_time = 0.12;
	var weapon_lifetime = 5;
	var weapon_projectile_scale = 1.2;
	
	//Player skill43 is active cooldown timer
	//Player skill44 is possible projectile bounce count
	
	STRING *str_weapon_code = "€";
	
	FONT *monoid = "Monoid#32b";
	
	TEXT *txt_fragment =
	{
		font = monoid;
		pos_x = 16;
		pos_y = 16;
		flags = CENTER_X | CENTER_Y | SHOW;
		strings = 1;
		string(str_weapon_code);
		red = 255;
		green = 128;
		blue = 255;
	}

	#include "weapon.c"

#endif
#ifndef WEAPON_H
#define WEAPON_H

	void shoot();
	void projectile();
	
	var weapon_speed = 32;
	var weapon_angle_correction = 0;
	var weapon_projectile_spawn_offset = 16;
	var weapon_cooldown_time = 0.12;
	
	//Player skill43 is active cooldown timer
	//Player skill44 is possible projectile bounce count

	#include "weapon.c"

#endif
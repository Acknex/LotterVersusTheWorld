#ifndef WEAPON_H
#define WEAPON_H

	void shoot();
	void projectile();
	
	var weapon_speed = 32;
	var weapon_angle_correction = 135;
	var weapon_projectile_spawn_offset = 16;
	var weapon_cooldown_time = 0.12;

	#include "weapon.c"

#endif
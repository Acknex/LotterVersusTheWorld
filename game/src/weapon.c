void weapon_startup()
{
	while(1)
	{
		if(player && mouse_left) 
		{
			player.skill44 = 2;
			shoot();
		}
		wait(1);		
	}
}


void ray_timer()
{
	var t = 0;
	
	vec_set(my.pan, player.pan);
	
	my.flags |= (FLAG2 | PASSABLE);
	
	VECTOR dir;
	VECTOR offset;
	VECTOR to;
	vec_for_angle(dir, player.pan);
	vec_set(offset, dir);
	vec_scale(dir, 10);
	
	vec_scale(offset, 5.5);
	vec_add(my.x, offset);
	
	my.tilt = 90;
	my.pan += 90;
	
	vec_scale(my.scale_x, 0.45);
	c_setminmax(me);
	my.skill50 = 0;
	
	var dist = 2;
	while(1)
	{
		t += time_step / 16;
		//dist = c_move(me, nullvector, dir, ACTIVATE_SHOOT | IGNORE_PASSABLE);
		
		
		my.x += dir.x * time_step;
		my.y += dir.y * time_step;
		my.z += dir.z * time_step;
		
		vec_set(to, dir);
		vec_normalize(to, 1);
		vec_set(offset, to);
		vec_scale(to, 16);
		vec_scale(offset, 4);
		vec_add(to, my.x);
		vec_add(offset, my.x);
		dist = c_trace(offset, to, IGNORE_ME);
		
		draw_line3d(to, NULL, 100);
		draw_line3d(offset, COLOR_GREEN, 100);
		draw_line3d(to, COLOR_GREEN, 100);
		
		if(you == player)
		{
			break;
		}
		
		if((dist != 0 || t > 5) && player.skill44 == 0 ) 
		{ 
			break; 
		}
		else if( dist != 0 && player.skill44 > 0 && my.skill50 < player.skill44 )
		{
			vec_set(dir, bounce);
			vec_scale(dir, 10);
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
		while(player.skill43 < 0.12 && mouse_left)
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
		ent_create("billboard.tga", player.x, ray_timer);
		cooldown();
	}
	
	//vec_for_angle(to, player.pan);
	//vec_scale(to, 1000);
	//vec_add(to, player.x);
}
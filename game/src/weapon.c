void ray_timer()
{
	var t = 0;
	
	vec_set(my.pan, player.pan);
	
	my.flags |= (FLAG2);
	
	VECTOR dir;
	VECTOR offset;
	vec_for_angle(dir, player.pan);
	vec_set(offset, dir);
	vec_scale(dir, 2);
	
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
		dist = c_move(me, nullvector, dir, IGNORE_FLAG2 | ACTIVATE_SHOOT );
		if((dist <= 1 || t > 5) && player.skill44 == 0 ) 
		{ 
			break; 
		}
		else if( dist <= 1 && player.skill44 == 1 && my.skill50 < player.skill44 )
		{
			vec_set(dir, bounce);
			my.skill50 += 1;
		} 
		else if (dist <= 1 || t > 5)
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
	
	player.flags |= (FLAG2);
	player.skill44 = 1;
	
	
	if(player.skill43 == 0)
	{
		ent_create("billboard.tga", player.x, ray_timer);
		cooldown();
	}
	
	vec_for_angle(to, player.pan);
	vec_scale(to, 1000);
	vec_add(to, player.x);
	
	draw_line3d(player.x, NULL, 100);
	draw_line3d(player.x, COLOR_GREEN, 100);
	draw_line3d(to, COLOR_GREEN, 100);
 }
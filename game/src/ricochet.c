void RICOCHET__effect()
{
	var t = 0;
	my.flags |= LIGHT | PASSABLE | TRANSLUCENT;
	
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
		//DEBUG_VAR(t,150);
		wait(1);
	}
	ptr_remove(my);
}

void RICOCHET_create(ENTITY* ent)
{
	VECTOR* v = vector(hit.nx, hit.ny, hit.nz);
	vec_normalize(v, 1);
	vec_add(v.x, hit.x);
	ENTITY* ricochet;
	
	//walls get special ricochet
	var vIsWall = 0;
	if (ent != NULL)
	{
		if (ent->type == TypeWall)
		{
			vIsWall = 1;
		}
	}
	
	if (vIsWall != 0)
	{
		ricochet = ent_create("ricochet.tga", v, RICOCHET__effect);
	}
	else
	{
		ricochet = ent_create("ricochet_red.tga", v, RICOCHET__effect);
	}
	
	vec_to_angle(ricochet->pan, vector(hit.nx, hit.ny, hit.nz));	
}
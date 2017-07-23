#ifndef LEVEL_FURNITURE_H
#define LEVEL_FURNITURE_H

action screenOnWall()
{
	my->skill41 = floatv(1);
	my->material = BlinkingObjectMaterial;
	set(me, PASSABLE);
}

action rack_tapewheel()
{
	my->roll = random(360);
	while (1)
	{
		my->roll += time_step * 10;
		wait(1);
	}
}

action rack_buildup()
{
	wait(1);
	my->skill41 = floatv(31);
	my->pan = my->skill1;
	set(my, PASSABLE);
	my->material = ObjectMaterial;
	random_seed(timer());
	var z = 5;
	while (z < 125)
	{
	ENTITY* model;
		var r = random(100);
		if ((125 - z) >= 40)
		{
			if (r < 15)
			{
				model = ent_create("rack_taperec.mdl", _vec(my->x, my->y, my->z + z), 0);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				model = ent_create("rack_taperec_wheel.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(50, 14, z + 20), _vec(my->skill1, 0, 0))), rack_tapewheel);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				model = ent_create("rack_taperec_wheel.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(50, -14, z + 20), _vec(my->skill1, 0, 0))), rack_tapewheel);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				z += 40;
				continue;
			}
			if (r < 30)
			{
				model = ent_create("rack_console.mdl", _vec(my->x, my->y, my->z + z), 0);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				model = ent_create("rack_console_keyboard.mdl", _vec(my->x, my->y, my->z + z), 0);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				z += 40;
				continue;
			}
		}
		if ((125 - z) >= 20)
		{
			if (r < 70)
			{
				model = ent_create("rack_hdd.mdl", _vec(my->x, my->y, my->z + z), 0);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				z += 20;
				continue;
			}
			else
			{
				model = ent_create("rack_memory.mdl", _vec(my->x, my->y, my->z + z), 0);
				model->pan = my->skill1;
				model->skill41 = floatv(1);
				model->material = BlinkingObjectMaterial;
				set(model, PASSABLE);
				z += 20;
			}
		}
	}
}

action desk_buildup()
{
	wait(1);
	my->pan = my->skill1;
	my->material = ObjectMaterial;
	set(my, PASSABLE);
	ENTITY* keyboard = ent_create("rack_console_keyboard.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(-11, -3, 68), _vec(my->skill1, 0, 0))), 0);
	keyboard->pan = -5 + my->skill1;
	keyboard->skill41 = floatv(1);
	keyboard->material = BlinkingObjectMaterial;
	set(keyboard, PASSABLE);

	ENTITY* screen = ent_create("desk_screen.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, 10, 84), _vec(my->skill1, 0, 0))), 0);
	screen->pan = -15 + my->skill1;
	screen->skill41 = floatv(1);
	screen->material = BlinkingObjectMaterial;
	set(screen, PASSABLE);

	ENTITY* lamp = ent_create("desk_lamp.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, -50, 70), _vec(my->skill1, 0, 0))), 0);
	lamp->pan = 50 + my->skill1;
	lamp->skill41 = floatv(31);
	lamp->material = ObjectMaterial;
	set(lamp, PASSABLE);

	ENTITY* phone = ent_create("desk_phone.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, -80, 80), _vec(my->skill1, 0, 0))), 0);
	phone->pan = 20+ my->skill1;
	phone->skill41 = floatv(31);
	phone->material = ObjectMaterial;
	set(phone, PASSABLE);

	ENTITY* rack = ent_create("rack_case.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(0, 65, 0), _vec(my->skill1, 0, 0))), rack_buildup);
	rack->skill1 = my->skill1;
	rack->skill41 = floatv(31);
	rack->material = ObjectMaterial;
	set(rack, PASSABLE);
}

#endif

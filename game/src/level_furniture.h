#ifndef LEVEL_FURNITURE_H
#define LEVEL_FURNITURE_H

action rack_tapewheel()
{
    my->roll = random(360);
    while (me)
    {
        my->roll += time_step * 10;
        wait(1);
    }
}

action rack_buildup()
{
    wait(1);
    my->pan = my->skill1;
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
                model = ent_create("rack_taperec_wheel.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(50, 14, z + 20), _vec(my->skill1, 0, 0))), rack_tapewheel);
		model->pan = my->skill1;
                model = ent_create("rack_taperec_wheel.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(50, -14, z + 20), _vec(my->skill1, 0, 0))), rack_tapewheel);
		model->pan = my->skill1;
                z += 40;
                continue;
            }
            if (r < 30)
            {
                model = ent_create("rack_console.mdl", _vec(my->x, my->y, my->z + z), 0);
		model->pan = my->skill1;
                model = ent_create("rack_console_keyboard.mdl", _vec(my->x, my->y, my->z + z), 0);
		model->pan = my->skill1;
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
                z += 20;
                continue;
            }
            else
            {
                model = ent_create("rack_memory.mdl", _vec(my->x, my->y, my->z + z), 0);
		model->pan = my->skill1;
                z += 20;
            }
        }
    }
}

action desk_buildup()
{
wait(1);
	my->pan = my->skill1;

    ENTITY* keyboard = ent_create("rack_console_keyboard.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(-11, -3, 68), _vec(my->skill1, 0, 0))), 0);
    keyboard->pan = -5 + my->skill1;

    ENTITY* screen = ent_create("desk_screen.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, 10, 84), _vec(my->skill1, 0, 0))), 0);
    screen->pan = -15 + my->skill1;

    ENTITY* lamp = ent_create("desk_lamp.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, -50, 70), _vec(my->skill1, 0, 0))), 0);
    lamp->pan = 50 + my->skill1;

    ENTITY* phone = ent_create("desk_phone.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(20, -80, 80), _vec(my->skill1, 0, 0))), 0);
    phone->pan = 20+ my->skill1;

    ENTITY* rack = ent_create("rack_case.mdl", vec_add(_vec(my->x, my->y, my->z), vec_rotate(_vec(0, 65, 0), _vec(my->skill1, 0, 0))), rack_buildup);
	rack->skill1 = my->skill1;
}

#endif

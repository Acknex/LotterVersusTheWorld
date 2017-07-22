#include <particles.c>

void p_bat_explode_fade(PARTICLE* p)
{
	if(p.lifespan < 6) p.size = p.skill_a*p.lifespan/6.0;
	if(p.z < 0 && p.vel_z < 0)
	{
		p.vel_x *= 0.8;
		p.vel_y *= 0.8;
		p.vel_z *= -1;
	}
}

void p_bat_explode(PARTICLE* p)
{
	vec_randomize(p.vel_x,32);
	vec_set(p.blue,vector(190,16,220));
	vec_scale(p.blue,random(0.2)+0.8);
	set(p,MOVE | BRIGHT);
	p.lifespan = 10+random(10);
	p.alpha = 50+random(50);
	p.gravity = 5;
	p.size = p.skill_a = random(8)+8;
	p.event = p_bat_explode_fade;
}



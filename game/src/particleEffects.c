#include <particles.c>

int p_event_do_wall_collision(PARTICLE* p)
{
	if(p.z < 0 && p.vel_z < 0)
	{
		p.vel_x *= 0.8;
		p.vel_y *= 0.8;
		p.vel_z *= -1;
	}

	p.skill_z = maxv(p.skill_z-1,0); // skip an additional frame
	if(LEVEL__stage && !p.skill_z)
	{
		int x,y;
		x = (p.x+100)/200;
		y = (p.y+100)/200;
		if(x > 0 && x < LEVEL__stage->size[0]-1 && y > 0 && y < LEVEL__stage->size[1]-1)
		{
			if(x != p.skill_x || y != p.skill_y)
			{
				TILE* tile = stageGetTile(LEVEL__stage, x,y);
				if(tile->value == TILE_EMPTY)
				{
					if(x != p.skill_x)
					{
						p.vel_x *= -0.9;
						p.x += p.vel_x;
					}
					if(y != p.skill_y)
					{
						p.vel_y *= -0.9;
						p.y += p.vel_y;
					}
					p.vel_z *= 1.2;
					p.skill_x = (int)((p.x+100)/200);
					p.skill_y = (int)((p.y+100)/200);
					p.skill_z = 2;  // skip an additional frame for next calculation because of abs(0.9) < 1
					return 1;
				}
			}
		}
	}
	return 0;
}
void p_bat_wall_bounce_highlight_fade(PARTICLE* p)
{
	p.size = p.skill_a*p.lifespan/3.0;
}

void p_bat_wall_bounce_highlight(PARTICLE* p)
{
	vec_set(p.blue,vector(220,64,250));
	vec_scale(p.blue,random(0.1)+0.9);
	set(p,BRIGHT);
	p.lifespan = 4;
	p.alpha = 50+random(50);
	p.size = p.skill_a = random(8)+16;
	p.event = p_bat_wall_bounce_highlight_fade;
}


void p_bat_explode_fade(PARTICLE* p)
{
	if(p_event_do_wall_collision(p))
	{
		effect(p_bat_wall_bounce_highlight,1,p.x,nullvector);
	}
	if(p.lifespan < 6) p.size = p.skill_a*p.lifespan/6.0;
	else
	{
		int x = (p.lifespan-6)/(p.skill_b-6)+1;
		p.size = p.skill_a*x*x;
	}
}

void p_bat_explode(PARTICLE* p)
{
	vec_randomize(p.vel_x,52);
	vec_set(p.blue,vector(190,16,220));
	vec_scale(p.blue,random(0.2)+0.8);
	set(p,MOVE | BRIGHT);
	p.lifespan = p.skill_b = 8+random(8);
	p.alpha = 50+random(50);
	p.gravity = 5;
	p.size = p.skill_a = random(8)+8;
	if(LEVEL__stage)
	{
		p.skill_x = (int)((p.x+100)/200);
		p.skill_y = (int)((p.y+100)/200);
	}
	p.event = p_bat_explode_fade;
}


void p_granate_explode_fade(PARTICLE* p)
{
	if(p_event_do_wall_collision(p))
	{
		effect(p_bat_wall_bounce_highlight,1,p.x,nullvector);
	}
	if(p.lifespan < 6) p.size = p.skill_a*p.lifespan/6.0;
	else
	{
		float x = (p.lifespan-6)/(p.skill_b-6)*2+1;
		p.size = p.skill_a*x*x;
	}
}

void p_granate_explode(PARTICLE* p)
{
	vec_randomize(p.vel_x,28);
	p.vel_z += random(50);
	vec_set(p.blue,vector(81,181,255));
	vec_scale(p.blue,random(0.2)+0.8);
	set(p,MOVE | BRIGHT);
	p.lifespan = p.skill_b = 8+random(8);
	p.alpha = 50+random(50);
	p.gravity = 5;
	p.size = p.skill_a = random(8)+8;
	if(LEVEL__stage)
	{
		p.skill_x = (int)((p.x+100)/200);
		p.skill_y = (int)((p.y+100)/200);
	}
	p.event = p_granate_explode_fade;
}



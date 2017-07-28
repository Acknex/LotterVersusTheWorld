#define TURRET_ATTACKRANGE 500
#define TURRET_ATTACK_STOP_RANGE 800
#define TURRET_SHOOTDELAY 3
#define TURRET_TURNSPEED 5
#define TURRET_ANIMOPENSPEED 5
#define TURRET_ANIMCLOSESPEED 7
#define TURRET_ANIMDIESPEED 8
#define TURRET_ALTERNATIONTIME 64

#define shootAngle skill24
#define turretState skill25
#define turretToggle skill26
#define turretRotationMode skill27
#define turretRotationTimer skill28

#define TURRETOPEN 0
#define TURRETCLOSE 1
#define TURRETACTIVE 2
#define TURRETDIE 3
#define TURRETSLEEP 4

#define TURRETTURNCCW 0
#define TURRETTURNCW 1
#define TURRETTURNALT 2
#define TURRETTURNAIM 3

#include "enemy.h"
#include "marker.h"
#include "entity_defs.h"
#include "stats.h"

void TURRET__init();
void TURRET__loop();
void TURRET__shoot();
void TURRET__event();
void TURRET__turnOn();
void TURRET__turnOff();
void TURRET__active();
void TURRET__die();
void TURRET__sleep();

SOUND* sndTurretUp = "turret_up.wav";
SOUND* sndTurretDown = "turret_down.wav";
SOUND* sndTurretDestroyed = "turret_destroyed.wav";
SOUND* sndTurretShot = "turret_shot.wav";


action enemy_turret_rotccw()
{
	TURRET__init();
	my->turretRotationMode = TURRETTURNCCW;
}

action enemy_turret_rotcw()
{
	TURRET__init();
	my->turretRotationMode = TURRETTURNCW;
}

action enemy_turret_alternate()
{
	TURRET__init();
	my->turretRotationMode = TURRETTURNALT;
}

action enemy_turret_aim()
{
	TURRET__init();
	my->turretRotationMode = TURRETTURNAIM;
	my->type = TypeTurret2;
}

void TURRET__init()
{
	ENEMY_init();
	my->health = 15;
	my->delayCounter = 0;
	my->type = TypeTurret;
	my->turretState = TURRETSLEEP;
	set(my, PASSABLE | POLYGON | FLAG1);
	ent_animate(my, "closed", 0, 0);

	TURRET__loop();
}

void TURRET__loop()
{
	while(player == NULL)
	{
		wait(1);
	}
	
	while (!is(my, dead))
	{
		switch(my->turretState)
		{
			case TURRETOPEN:
				TURRET__turnOn();
				break;
			
			case TURRETCLOSE:
				TURRET__turnOff();
				break;
			
			case TURRETACTIVE:
				TURRET__active();
				break;
			
			case TURRETSLEEP:
				TURRET__sleep();
				break;
			
			default:
				break;
		}
		enemy_do_hit_flash(my);
		if(cheats_enabled && key_k) {
			my.health = 0;
			break;
		}
		wait(1);	
	}
	my.skill44 = floatv(0);
	TURRET__die();
}

void TURRET__turnOn()
{
	reset(my, PASSABLE); // Active turrets must be damageable
	MARKER_update(me);
	my->animCounter += TURRET_ANIMOPENSPEED * time_step;
	if (my->animCounter >= 100)
	{
		my->turretState = TURRETACTIVE;
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", my->animCounter, 0);
	my->skill41 = floatv(0.01 * my->animCounter);
}

void TURRET__turnOff()
{
	MARKER_update(me);
	my->animCounter += TURRET_ANIMCLOSESPEED * time_step;
	if (my->animCounter >= 100)
	{
		set(my, PASSABLE); // Inactive turrets are immune
		my->turretState = TURRETSLEEP;
	}
	my->animCounter = minv(100, my->animCounter);
	ent_animate(me, "open", 100 - my->animCounter, 0);
	if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
	{
		my->turretState = TURRETOPEN;
		snd_play(sndTurretUp, 100, 0);
		my->animCounter = 100 - my->animCounter;
	}
	my->skill41 = floatv(0.01 * (100 - my->animCounter));
}

void TURRET__active()
{
	MARKER_update(me);
	my->delayCounter += time_step;
	if (vec_dist(player->x, my->x) > TURRET_ATTACK_STOP_RANGE)
	{
		snd_play(sndTurretDown, 100, 0);
		my->turretState = TURRETCLOSE;
		my->animCounter = 0;
	}
	else 
	{
		if (my->delayCounter >= TURRET_SHOOTDELAY)
		{
			TURRET__shoot();
		}
		var vTurnStep = TURRET_TURNSPEED * time_step;
		switch (my->turretRotationMode)
		{
			case TURRETTURNCCW:
				break;

			case TURRETTURNCW:
				vTurnStep *= -1;
				break;

			case TURRETTURNALT:
				my->turretRotationTimer = cycle(my->turretRotationTimer + time_step, 0, TURRET_ALTERNATIONTIME);
				if (my->turretRotationTimer > TURRET_ALTERNATIONTIME * 0.5)
				{
					vTurnStep *= -2;
				}
				else
				{
					vTurnStep *= 2;
				}
				break;

			case TURRETTURNAIM:
				break;

			default:
				break;
		}
		if (my->turretRotationMode == TURRETTURNAIM)
		{
			VECTOR vecDist;
			vec_set (&vecDist, player->x);
			vec_sub(&vecDist, my->x);
			vecDist.z = my->z;
			ANGLE angTemp;
			vec_to_angle(&angTemp, &vecDist);
			my->shootAngle = 180 + angTemp.pan;
			ent_bonereset(me, "Bone1");
			ent_bonerotate(me, "Bone1", vector(my->shootAngle, 0, 0));
		}
		else
		{
			my->shootAngle += vTurnStep;
			ent_bonerotate(me, "Bone1", vector(vTurnStep, 0, 0));
		}

		my->delayCounter = cycle(my->delayCounter, 0, TURRET_SHOOTDELAY);		
	}
}

void TURRET__die()
{
	snd_play(sndTurretDestroyed, 100, 0);
	my->event = NULL;
	my->animCounter = 0;
	my->type = TypeDestroyed;
	
	stats_addKill(STATENEMY_TURRET);
	
	while (my->animCounter < 100)
	{
		effect(p_spark, 1, my.x, vector(6,6,6));
		my->animCounter += TURRET_ANIMDIESPEED * time_step;
		my->animCounter = minv(100, my->animCounter);
		ent_animate(me, "die", my->animCounter, 0);	
		ent_bonerotate(me, "Bone1", vector(my->shootAngle, 0, 0));
		MARKER_update(me);
		wait(1);
	}
	
	set(my, PASSABLE);
	while (1)
	{
		if(random(100) < 5) {
			effect(p_spark, 1, my.x, vector(6,6,6));
			my->skill41 = floatv(0.0);
		} else {
			my->skill41 = floatv(1.0);
		}
		MARKER_update(me);
		wait(1);
	}
		
//	ptr_remove(me);
}

void TURRET__sleep()
{
	my->event = NULL;
	if (vec_dist(player->x, my->x) < TURRET_ATTACKRANGE)
	{
		my->turretState = TURRETOPEN;
		snd_play(sndTurretUp, 100, 0);
		my->animCounter = 0;
		my->event = TURRET__event;
	}
	
	my->skill41 = floatv(clamp(0.5 * sinv(10 * total_ticks), 0, 1));
}

void TURRET__shoot()
{
	if (my->turretToggle != 584)
		my->turretToggle = 584;
	else
		my->turretToggle = 592;
	
	VECTOR vecPos;
	vec_for_vertex(&vecPos, me, my->turretToggle);
	VECTOR* vecDist = vector(-30, 0, 0);
	ANGLE* angRot = vector(my->shootAngle, 0, 0);
	vec_rotate(vecDist, angRot);
	vec_add (vecDist, &vecPos);
	snd_play(sndTurretShot, 100, 0);
	ENTITY* ent = ent_create(SPHERE_MDL, vecDist, enemy_projectile);	
	ent->damage = 9;
	ent->pan = my->shootAngle;	
}

void TURRET__event()
{
	var vDamageDealt = ENEMY_hit(event_type);
}

// effects
var p_spark_size_factor = 2;

void p_spark_fade(PARTICLE* p)
{
	p.vel_x *=1-0.09 * time_step;
	p.vel_z -=3 * time_step;
	p.x +=0.8 * p.vel_x * time_step;
	p.y -= 2 * time_step;
	p.z += 0.8 * p.vel_z*time_step;
}

void p_spark(PARTICLE* p) {
	p.skill_a = random(360);
	p.skill_b = 17 + random(12);
	p.vel_x = sinv(p.skill_a) * p.skill_b;
	p.vel_z = cosv(p.skill_a) * p.skill_b;
	vec_set(p.blue, vector(255, 140, 160 * random(80)));
	if (random(2) > 1) {
		set(p,BEAM);
	} else {
		set(p,STREAK);
	}
	p.bmap = bmapSpark;
	p.size = (1 + random(2)) * p_spark_size_factor;
	p.alpha = 100;
	p.lifespan = 4+random(3);
	p.event = p_spark_fade;
}

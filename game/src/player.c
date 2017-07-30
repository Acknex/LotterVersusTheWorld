#include "camera.h"
#include "datatypes.h"
#include "levelgen.h"
#include "entity_defs.h"
#include "cheats.h"
#include "stats.h"

//STAGE* LEVEL__stage = NULL;
#define slowdown FLAG8
#define jumping FLAG7

var anim_percentage = 0;
var dist_ahead = 0;
var dist_strafe = 0;
VECTOR playerpos, temp;
ANGLE diff, mouseDir, moveDir;
var playerVelY = 0;
var shootingHandle = 0;
var jumpbuttonHit = 0;

var desyncTimer = 0;

var handleSndEngineIdle = 0;
var handleSndEngineThrust = 0;

SOUND* snd_engine_idle = "engine_idle.wav";
SOUND* snd_engine_thrust = "engine_thrust.wav";

SOUND* snd_player_hit1 = "player_hit1.wav";
SOUND* snd_player_hit2 = "player_hit2.wav";
SOUND* snd_player_hit3 = "player_hit3.wav";

int tutorialStage = 0;
var tutorialTimer = 0;

/*
void player_move_old() {
	
	if (mouse_mode > 0)	
	{ 
		mouse_pos.x = mouse_cursor.x;    
		mouse_pos.y = mouse_cursor.y;
	}
	
	VIEW* view = get_camera();
	vec_set(playerpos, player->x);
	
	VECTOR vTarget;
	VECTOR to;
	ANGLE tAngle;
	vec_set(to,mouse_dir3d);
	vec_scale(to,5000); // set a range
	vec_add(to, mouse_pos3d);
	you = player;
	c_trace(mouse_pos3d, to, IGNORE_YOU | IGNORE_FLAG2);
	
	//draw_point3d(to, COLOR_WHITE, 100, 20);
	
	vec_set(vTarget, hit.x);
	vec_sub(vTarget, player.x);
	vec_to_angle(tAngle, vTarget);
	player.pan = tAngle.pan;

	dist_ahead = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (clamp(key_w + key_cuu, 0, 1) - clamp(key_s + key_cud, 0, 1));
	dist_strafe = (PLAYER_WALK_SPPED + key_shiftl*PLAYER_RUN_SPEED) * (clamp(key_a + key_cul, 0, 1) - clamp(key_d + key_cur, 0, 1));
	if (dist_ahead != 0 && dist_strafe != 0)
	{
		dist_ahead /= sqrt(2);
		dist_strafe /= sqrt(2);
	}
	VECTOR* vecDir = vector(dist_ahead, dist_strafe, 0);
	vec_scale(vecDir, time_step);
	
	//transform with camera angle
	vec_rotate(vecDir, vector(view->pan, 0, 0));
	
	if (move_style == 0) {
		c_ignore(4,0);
		c_move(player, nullvector, vecDir, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
		} else {
		c_ignore(4,0);
		c_move(player, vector(dist_ahead * time_step, 0, 0), nullvector, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
	}
	
	// animation
	if (dist_ahead != 0 || dist_strafe != 0) {
		if (key_shiftl) {
			// run
			anim_percentage += 0.6*maxv(abs(dist_ahead), abs(dist_strafe)) * time_step;
			ent_animate(player,"walk", anim_percentage, ANM_CYCLE);
			} else {
			//walk
			anim_percentage += 0.6*maxv(abs(dist_ahead), abs(dist_strafe)) * time_step;
			ent_animate(player,"walk",anim_percentage,ANM_CYCLE);
		}	
		
		// If walk position and mouse position = 90° -> strafe 100%
		// if walk position and mouse position 0 0° -> walk 100%
		vec_to_angle(moveDir, vecDir);
		ang_diff(diff, player.pan, moveDir.pan);
		
		// diff.pan = 0,180 -> walk, 90,270->strafe
		draw_textmode("Arial", 1, 20, 100);
		DEBUG_VAR(cycle(diff.pan, 0, 360), 10);

		var percentage = (1-abs(abs(ang(diff.pan))-90.0)/90.0)*100;
		DEBUG_VAR(percentage, 30);
		
		ent_blendframe(player, player, "strafe", 0, percentage);
		
		
		} else {
		anim_percentage += 5*time_step; 
		ent_animate(player,"stand",anim_percentage,ANM_CYCLE);
	}
	
	MARKER_update(player);
	if(LEVEL__stage) 
	{
		static int playerX = 0;
		static int playerY = 0;
		int x,y;
		stageGetIndicesFromPos(LEVEL__stage,player.x,&x,&y);
		if(playerX != x || playerY != y)
		{
			playerX = x;
			playerY = y;		
			stageDoFlood(LEVEL__stage, playerX, playerY, FLOOD_PLAYER, 12, 0);
		}
	}
}
*/

void player_hit_sound() {
	var i = integer(random(3));
	if(i == 0)
	{
		snd_play(snd_player_hit1, 100, 0);
	}
	else if(i == 1)
	{
		snd_play(snd_player_hit2, 100, 0);
	}
	else if(i == 2)
	{
		snd_play(snd_player_hit3, 100, 0);
	}
}

void hit_player(var dmg)
{
	if(player != NULL && player->health > 0)
	{
		player->health -= dmg;
		player_hit_sound();
		desyncTimer = 0.4; // 0.4 second desync
		if (player->health <= 0) 
		{
			pp_desync(0);
			if(handleSndEngineIdle != 0)
			{
				snd_stop(handleSndEngineIdle);
				handleSndEngineIdle = 0;
			}
			if(handleSndEngineThrust != 0)
			{
				snd_stop(handleSndEngineThrust);
				handleSndEngineThrust = 0;
			}
			if(shootingHandle != 0)
			{
				snd_stop(shootingHandle);
				shootingHandle = 0;
			}
			
		}
	}
}

STRING *player_infoText = "#256";

void player_move() {
	if(player->health <= 0)
		return;
		
	if (mouse_mode > 0)	
	{ 
		mouse_pos.x = mouse_cursor.x;    
		mouse_pos.y = mouse_cursor.y;
	}
	
	VIEW* view = get_camera();
	vec_set(playerpos, player->x);
	
	VECTOR vTarget,to,temp,temp2;
	ANGLE tAngle;
	vec_set(to,mouse_dir3d);
	vec_scale(to,5000); // set a range
	vec_add(to, mouse_pos3d);
	you = player;
	c_trace(mouse_pos3d, to, IGNORE_YOU | IGNORE_FLAG2);
	//draw_point3d(target, COLOR_WHITE, 100, 16);
	
	vec_diff(temp,target,player.x);
	if(!player.near_teleport && vec_to_angle(temp2,temp) > 8)
	{
		var diff = ang(temp2.x-player.pan);
		static var diffAlignSpeed = 0;
		diffAlignSpeed = minv(diffAlignSpeed+2*time_step,minv(abs(diff),25));
		player.pan += clamp(diff*0.35,-diffAlignSpeed,diffAlignSpeed)*time_step;
	}
	
	if(mouse_middle && cheats_enabled) {
		vec_set(player.x, target);
	}
	
	if(player.pause_control == 0)
	{
		static VECTOR vPlayerSpeed;

		vec_set(temp,vector(sign(key_w-key_s+key_cuu-key_cud),sign(key_a-key_d+key_cul-key_cur),0));
		VIEW* view = get_camera();
		vec_rotate(temp,vector(view->pan,0,0));
		if(temp.x || temp.y) vec_normalize(temp,45);
		vec_diff(temp2,temp,vPlayerSpeed);
		vec_normalize(temp2,minv(10,vec_length(temp2))*0.3*time_step);
		//vec_lerp(vPlayerSpeed,vPlayerSpeed,temp,0.2*time_step);
		vec_add(vPlayerSpeed,temp2);
		VECTOR vSpeed;
		vec_set(&vSpeed, vPlayerSpeed);
		vSpeed.z = 0;
		//hole slows down player - if he's not jumping
		if (is(player, slowdown) && !is(player, jumping))
		{
			vec_scale(&vSpeed, 0.4);
//			reset(player, slowdown);
		}
		vec_scale(&vSpeed, time_step);
		c_ignore(4,0);
		//c_move(player, nullvector, vector(vPlayerSpeed.x*time_step,vPlayerSpeed.y*time_step,0), IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
		c_move(player, nullvector, &vSpeed, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
		
		var engineVolume = vec_length(vPlayerSpeed)/45;
		snd_tune(handleSndEngineThrust, engineVolume*80, engineVolume*70, 0);
		snd_tune(handleSndEngineIdle, (1-engineVolume)*100, 0, 0);
		
		if(player.near_teleport == 0) {
		
			if(!jumpbuttonHit && key_space && player.z <= 190 && !is(player, slowdown)) {
				playerVelY = 10;
				set(player, jumping);
			}
			jumpbuttonHit = key_space;
		
			// Fancy mini-gravity
			player.z += playerVelY * time_step;
			if(player.z <= 190) {
				reset(player, jumping);
				playerVelY = abs(0.3 * playerVelY);
				player.z = 190;
				if(abs(playerVelY) < 0.05) {
					playerVelY = 0;
				}
			} else {
				playerVelY -= 1.5 * time_step;
			}
		}
		var len = vec_length(vPlayerSpeed);
		if(HIT_TARGET && len > 15)
		{
			bounce.z = 0;
			vec_normalize(bounce,len*0.45);
			vPlayerSpeed.x = bounce.x;
			vPlayerSpeed.y = bounce.y;
		}
		ent_animate(player,"attack",0,0);
		ent_bonerotate(player,"Bone1",vector(0,sinv(total_ticks*8)*10,0));
		ent_bonerotate(player,"Bone4",vector(0,sinv(total_ticks*8)*10,0));
		reset(player, slowdown);
		
	}
	int i;
	for(i = 0; i < 2; i++)
	{
		if(!player.skill[28+i])
		{
			you = ent_create("hoverboardGlow.mdl",player,NULL);
			your.material = HoveboardGlowMaterial;
			set(you,PASSABLE | UNLIT | TRANSLUCENT);
			player.skill[28+i] = you;
		}
		you = player.skill[28+i];
		STRING* str = str_printf(NULL,"Bone%d",(int)(22+i));
		vec_for_bone(your.x,player,str);
		ang_for_bone(your.pan,player,str);
		VECTOR temp,temp2;
		vec_set(temp,vector(0,0,-1));
		vec_rotate(temp,your.pan);
		vec_set(temp2,temp);
		vec_scale(temp2,10);
		vec_add(temp2,your.x);
		your.skill21 += time_step;
		if(your.skill21 > 0.5)
		{
			your.skill21 -= 0.5;
			effect(p_hoverboard_smoke,3,temp2,temp);
		}
	}
	/* SHOOOTING */
	
	if(mouse_left) 
	{
		if(shootingHandle == 0)
		{
			shootingHandle = snd_loop(sndPlayerShot, 50, 0);
		}
		shoot(1);
	}
	if(!mouse_left && shootingHandle != 0) 
	{
		snd_stop(shootingHandle);
		shootingHandle = 0;
	}
	if(mouse_right)
	{
		shoot(2);
	}
	
	//mark only enemies and items. no projectiles, no player
	//MARKER_update(player);
	if(LEVEL__stage) 
	{
		static int playerX = 0;
		static int playerY = 0;
		int x,y;
		stageGetIndicesFromPos(LEVEL__stage,player.x,&x,&y);
		if(playerX != x || playerY != y)
		{
			playerX = x;
			playerY = y;		
			stageDoFlood(LEVEL__stage, playerX, playerY, FLOOD_PLAYER, 12, 0);
		}
	}
	
	desyncTimer = maxv(0, desyncTimer - time_step/16);
	pp_desync(floatv(desyncTimer/0.4*20));
	
	// Don't reset this, we show tutorial only once per game instance
	switch(tutorialStage)
	{
		case 0:
			MARKER_setText(player, "WASD:\nMove");
			if(key_w || key_s || key_a || key_d || key_cul || key_cur || key_cuu || key_cud) {
				tutorialStage = 1;
			}
			break;
		case 1:
			MARKER_setText(player, "LMB:\nShoot");
			if(mouse_left) {
				tutorialStage = 2;
			}
			break;
		case 2:
			MARKER_setText(player, "RMB:\nGrenade");
			if(mouse_right) {
				tutorialStage = 3;
			}
			break;
		case 3:
			MARKER_setText(player, "Space:\nJump");
			if(key_space) {
				tutorialStage = 4;
				tutorialTimer = 8;
			}
			break;
		case 4:
		case 5:
		case 6:
			if(tutorialStage == 4)
				MARKER_setText(player, "KILL");
			else if(tutorialStage == 5)
				MARKER_setText(player, "THEM");
			else
				MARKER_setText(player, "ALL");
			if(tutorialTimer <= 0) {
				tutorialStage++;
				tutorialTimer = 8;
				break;
			}
			tutorialTimer -= time_step;
			break;
		default:
			int pre = stat_multFactor;
			int post = ((int)(10 * stat_multFactor)) % 10;
			MARKER_setText(player, str_printf(
				player_infoText,
				"LIV:%d\nSCO:%d\nMUL:x%1d.%1d",
				(int)(player.health),
				(int)(stats_current.score),
				pre, post));
			break;
	}
	
}


VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py);

void player_init() {
	player = ent_create("cbabe_maleHover.mdl", stageGetEntrancePos(LEVEL__stage, NULL, NULL, NULL), NULL);
	player->material = LotterMaterial;
	player->health = 100;
	player->weapon_bouncing = 2;
	player->group = 3;
	player.type = TypePlayer;
	
	// Adapt scale
	vec_scale(player.scale_x, 2.25);
	
	// Adapt bounding box
	c_setminmax(player);
	wait(1);
	vec_set(player.min_x, vector(-30, -30, -188));
	vec_set(player.max_x, vector(30, 30, 12));
	move_friction = 0.1;
	
	mouse_mode = 4;
	player.z = 150;
	player->trigger_range = 50;
	
	player.damage = 1;
	
	player.emask |= ENABLE_SHOOT | ENABLE_SCAN | ACTIVATE_SHOOT;
	player.event = player_event;
	player->type = TypePlayer;
	
	handleSndEngineIdle = snd_loop(snd_engine_idle, 100, 0);
	handleSndEngineThrust = snd_loop(snd_engine_thrust, 0, 0);
}

void player_event() {
	switch(event_type) {
		case EVENT_SHOOT:
			hit_player(your.damage);
			break;
		case EVENT_SCAN:
			if (your->type == TypeHole)
			{
				set(player, slowdown);
			}
			else
			{
				hit_player(your.damage);
			}
			break;
		
	}
}

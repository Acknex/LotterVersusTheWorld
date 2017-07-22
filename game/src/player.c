#include "camera.h"
#include "datatypes.h"
#include "levelgen.h"
#include "entity_defs.h"

//STAGE* LEVEL__stage = NULL;

var anim_percentage = 0;
var dist_ahead = 0;
var dist_strafe = 0;
VECTOR playerpos, temp;
ANGLE diff, mouseDir, moveDir;

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
		c_move(player, nullvector, vecDir, IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
		} else {
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

void player_move() {
	
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
	draw_point3d(target, COLOR_WHITE, 100, 16);
	
	vec_diff(temp,target,player.x);
	if(!player.near_teleport && vec_to_angle(temp2,temp) > 8)
	{
		var diff = ang(temp2.x-player.pan);
		static var diffAlignSpeed = 0;
		diffAlignSpeed = minv(diffAlignSpeed+2*time_step,minv(abs(diff),25));
		player.pan += clamp(diff*0.35,-diffAlignSpeed,diffAlignSpeed)*time_step;
	}
	
	if(mouse_middle) {
		vec_set(player.x, target);
	}
	
	if(player.pause_control == 0)
	{
		static VECTOR vPlayerSpeed;
		vec_set(temp,vector(key_w-key_s,key_a-key_d,0));
		VIEW* view = get_camera();
		vec_rotate(temp,vector(view->pan,0,0));
		if(temp.x || temp.y) vec_normalize(temp,45);
		vec_diff(temp2,temp,vPlayerSpeed);
		vec_normalize(temp2,minv(10,vec_length(temp2))*0.3*time_step);
		//vec_lerp(vPlayerSpeed,vPlayerSpeed,temp,0.2*time_step);
		vec_add(vPlayerSpeed,temp2);
		
		c_move(player, nullvector, vector(vPlayerSpeed.x*time_step,vPlayerSpeed.y*time_step,0), IGNORE_PASSABLE | GLIDE | ACTIVATE_TRIGGER);
		
		if(player.near_teleport == 0) {
			player.z = 190;
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


VECTOR* stageGetEntrancePos(STAGE* stage, VECTOR* vpos, int *px, int *py);

void player_init() {
	player = ent_create("cbabe_maleHover.mdl", stageGetEntrancePos(LEVEL__stage, NULL, NULL, NULL), NULL);
	player->material = LotterMaterial;
	
	// Adapt scale
	vec_scale(player.scale_x, 2.25);
	
	// Adapt bounding box
	c_setminmax(player);
	wait(1);
	vec_set(player.min_x, vector(-30, -30, -70));
	vec_set(player.max_x, vector(30, 30, 12));
	
	move_friction = 0.1;
	
	mouse_mode = 4;
	player.z = 150;
	player->trigger_range = 20;
	
	player.damage = 1;
	
	player.emask |= EVENT_SHOOT;
	player.event = player_event;
	player->type = TypePlayer;
}

void player_event() {
	switch(event_type) {
		case EVENT_SHOOT:
		my.health -=your.damage;
		if (my.health <= 0) {
			printf("You are DEAD!");
		}
		break;
	}
}

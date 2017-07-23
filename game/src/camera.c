/*
 *******************************************************************************
 * camera.c
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2014-07-28 22:11:39 +0200 (Mo, 28 Jul 2014) $
 * $Revision: 411 $
 * $Author: Firoball $
 *
 *******************************************************************************
 * Description
 *
 * Script for camera movement
 *
 * Comments
 * 
 * for short descriptions see comments in camera.h
 *
 *******************************************************************************
 */
 
#include "camera_cfg.h"

ENTITY* camera_focus_ent = NULL;
VIEW* cam;
var vDistanceFactor;
var camShakeTime = 0;
var camShakeTimeTarget = 0;
var vViewWidth;
VECTOR cameraTarget;

void create_camera()
{
	create_camera(1);
}

void create_camera(int layer)
{
	cam = view_create(layer);
	cam->pan = CAMERA_PAN;
	cam->tilt = CAMERA_TILT; 
	//cam->flags |= ISOMETRIC;

	/* width = view.size_x * 2 * tan(view.arc/2); - from manual (view.arc) */
	//vViewWidth = CAMERA_REFSCRSIZEX * 2 * tanv(CAMERA_REFARC * 0.5);
}

void remove_camera()
{
	if (cam != NULL)
	{
		ptr_remove(cam);
		cam = NULL;
	}	
}


void update_camera()
{
	VECTOR vecPos;

	if ((camera_focus_ent != NULL) && (cam != NULL))
	{
		vec_set(vecPos, vector(-CAMERA_DIST, 0, 0));
		vec_rotate(vecPos, vector(cam->pan,cam->tilt+3,0)); // +3 because not enough information visible on bottom part of screen
		vec_add(vecPos, camera_focus_ent->x);
		VECTOR tmp;
		var fac = clamp(time_step, 0, 1) * CAMERA_SPEEDFAC;
		vec_lerp(&tmp, cameraTarget.x, vecPos, fac);
		vec_set(cameraTarget.x, tmp);
		
		vec_set(cam->x, cameraTarget.x);
		camShakeTime = minv(camShakeTimeTarget, camShakeTime + time_step/16);
		if(camShakeTime < camShakeTimeTarget)
		{
			var shakeFactor = (camShakeTimeTarget-camShakeTime)/camShakeTimeTarget;
			tmp.x = sin(camShakeTime*4000)*shakeFactor*30;
			tmp.y = sin(camShakeTime*6000)*shakeFactor*30;
			tmp.z = sin(camShakeTime*8000)*shakeFactor*30;
			vec_add(cam->x, tmp.x);
		}

		//vDistanceFactor += ((is_kart_accelerating(camera_focus_ent) > 0) * 0.05 - 0.02) * time_step;
		//vDistanceFactor = clamp(abs(vDistanceFactor), 0.28, 0.6);

		/* view.arc = 2*atan(width/(view.size_x * 2)); - from manual (view.arc) */
		/* change camera arc by manipulating view size - delivers resolution independent result */
		//cam->arc = 2 * atanv(vViewWidth * 0.5 / (screen_size.x / vDistanceFactor));
	}
	
}

void shake_cam(var time)
{
	camShakeTimeTarget = time;
	camShakeTime = 0;
}
void show_camera()
{
	if (cam != NULL)
	{
		cam->flags |= SHOW;
		camera->flags &= ~SHOW;
	}
}

void hide_camera()
{
	if (cam != NULL)
	{
		camera->flags |= SHOW;
		cam->flags &= ~SHOW;
	}
}

void focus_camera(ENTITY* ent)
{
	camera_focus_ent = ent;
	if(camera_focus_ent) {
		vec_set(cam->x, camera_focus_ent->x);
		vec_set(cameraTarget, camera_focus_ent->x);
	}
}
 
VIEW* get_camera()
{
	return cam;
}


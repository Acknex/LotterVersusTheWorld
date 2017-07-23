/*
 *******************************************************************************
 * camera.h
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2014-07-20 01:07:16 +0200 (So, 20 Jul 2014) $
 * $Revision: 283 $
 * $Author: Firoball $
 *
 *******************************************************************************
 * Description
 *
 * definition script for camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
 
#ifndef CAMERA_H
#define CAMERA_H
 
 
void create_camera();
void create_camera(int);
void remove_camera();
void update_camera();
void show_camera();
void shake_cam(var shakeTime);
void hide_camera();
void focus_camera(ENTITY* ent);
VIEW* get_camera();
 
 
#include "camera.c"

#endif
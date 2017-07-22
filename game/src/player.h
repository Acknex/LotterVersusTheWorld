#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_WALK_SPPED 16
#define PLAYER_RUN_SPEED 8

var move_style = 0; // 0 = old, 1 = new

void player_move();
void player_init();
void player_event();

#include "player.c"

#endif
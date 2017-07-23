#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_WALK_SPPED 24
#define PLAYER_RUN_SPEED 16

var move_style = 0; // 0 = old, 1 = new

void player_move();
void player_init();
void player_event();
void hit_player(var dmg);

#include "player.c"

#endif
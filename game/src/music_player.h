#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

// starts new music, cross fades with old one over crossFadeTime (in seconds)
// if file is NULL, the current music is faded out
void startMusic(STRING* file, var crossFadeTime);

// stops all music immediately
void stopMusic();

// updates volumes. run in main loop
void updateMusic();

// 0..100
void setMusicMasterVolume(var vol);

#include "music_player.c"

#endif
#ifndef STATS_H
#define STATS_H

#define STATENEMY_BAT      0
#define STATENEMY_SPUTNIK  1
#define STATENEMY_SPHEREOD 2
#define STATENEMY_TURRET   3

int stats_scores[4] = { 10, 25, 15, 20 };

typedef struct
{
	var score;
	var kills[4];
} HIGHSCORE;

void stats_init();

void stats_reset();

void stats_addKill(int enemyType);

/**
 * Finalizes the stats and returns TRUE when a new highscore was reached
 */
BOOL stats_finalize();

#include "stats.c"

#endif
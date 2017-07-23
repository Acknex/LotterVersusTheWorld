#include <stdio.h>
#include "Annoying.h"

FONT * stats_font = "Arial#40b";

HIGHSCORE stats_highscore;
HIGHSCORE stats_current;

PANEL * panHighscores = 
{
	digits(0, 0, "Score: %.0f", stats_font, 1, stats_current.score);
	pos_x = 10;
	pos_y = 10;
}

void stats_init()
{
	stats_reset();
	
	FILE * f = fopen("highscores.dat", "rb");
	if(f != NULL) {
		fread(&stats_highscore, sizeof(HIGHSCORE), 1, f);
		fclose(f);
	}
	
	while(1)
	{
		if(INIT__levelRunning)
		{
			set(panHighscores, SHOW);
			DEBUG_VAR(stats_current.score, 16);
		}
		else
		{
			reset(panHighscores, SHOW);
		}
		wait(1);
	}
}

void stats_reset()
{
	memset(&stats_current, 0, sizeof(HIGHSCORE));
}

void stats_addKill(int enemyType)
{
	stats_current.score += stats_scores[enemyType];
	stats_current.kills[enemyType] += 1;
}

/**
 * Finalizes the stats and returns TRUE when a new highscore was reached
 */
BOOL stats_finalize()
{
	BOOL isSuper = (stats_current.score > stats_highscore.score);
	
	if(isSuper) {
		FILE * f = fopen("highscores.dat", "wb");
		fwrite(&stats_current, sizeof(HIGHSCORE), 1, f);
		fclose(f);
		
		memcpy(&stats_highscore, &stats_current, sizeof(HIGHSCORE));
	}
	
	return isSuper;
}
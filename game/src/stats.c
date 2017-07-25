#include <stdio.h>
#include "Annoying.h"

FONT * stats_font = "Arial#40b";

HIGHSCORE stats_highscore;
HIGHSCORE stats_current;

BOOL stat_gotHighscore;

float stat_multCounter = 0;
float stat_multFactor = 1;

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
			//DEBUG_VAR(stats_current.score, 16);
			
			if(stats_current.score > stats_highscore.score)
			{
				stat_gotHighscore = TRUE;
				memcpy(&stats_highscore, &stats_current, sizeof(HIGHSCORE));
			}
		}
		else
		{
			reset(panHighscores, SHOW);
		}
		stat_multCounter += time_step;
		if(stat_multCounter >= 96) { // 6 Sekunden bis zum Reset
			stat_multFactor = 1.0;
		}
		wait(1);
	}
}

void stats_reset()
{
	memset(&stats_current, 0, sizeof(HIGHSCORE));
	stat_gotHighscore = FALSE;
	stat_multCounter = 0;
	stat_multFactor = 1;
}

void stats_addKill(int enemyType)
{
	stats_current.score += stat_multFactor * stats_scores[enemyType];
	stats_current.kills[enemyType] += 1;
	
	stat_multFactor += 0.1;
	stat_multCounter = 0;
}

/**
 * Finalizes the stats and returns TRUE when a new highscore was reached
 */
BOOL stats_finalize()
{
	if(stat_gotHighscore) {
		FILE * f = fopen("highscores.dat", "wb");
		fwrite(&stats_current, sizeof(HIGHSCORE), 1, f);
		fclose(f);
	}
	return stat_gotHighscore;
}
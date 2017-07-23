#ifndef TURRET_H
#define TURRET_H

/*action enemy_turret();*/

void p_spark_fade(PARTICLE* p);
void p_spark(PARTICLE* p);
BMAP *bmapSpark = "spark.tga";

#include "turret.c"

#endif
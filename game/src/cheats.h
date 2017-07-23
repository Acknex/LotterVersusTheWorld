#ifndef CHEATS_H
#define CHEATS_H

SOUND *cheats_sndEnable = "sndSpawn.ogg";

var cheats_enabled = 0;

function cheats_startup()
{
	while(!key_j || !key_c || !key_l) wait(1);
	cheats_enabled = 1;
	snd_play(cheats_sndEnable, 100, 0);
}

#endif
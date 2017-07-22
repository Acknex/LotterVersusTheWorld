


action teleporter_out()
{
	set(me, PASSABLE);
	while(!player) wait(1);
	while(me)
	{
		var dist = vec_dist(player.x, me.x);
		DEBUG_VAR(dist, 512);
		if(dist < 170) // ist höhenabhängig!
		{
			error("?");
			return;
		}
		wait(1);
	}
}
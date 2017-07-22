
action enemy_spikes()
{
	set(me, POLYGON);
	set(me, FLAG1);
	my.type = 8;
	
	while(me)
	{
		my.skill41 = floatv(0.5 + 0.5 * sinv(total_ticks));
		ent_animate(me, "open", 50 + 50 * sinv(10 * total_ticks), ANM_CYCLE);
		MARKER_update(me);
		wait(1);
	}
}

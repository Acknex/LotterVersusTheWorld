

action enemy_hole()
{
	set(me, POLYGON | PASSABLE | FLAG1);
	my.type = 9;
	
	while(1)
	{
		my.skill41 = floatv(0.5 + 0.5 * sinv(total_ticks));
		ent_animate(me, "activate", total_ticks, ANM_CYCLE);
		MARKER_update(me);
		wait(1);
	}
}
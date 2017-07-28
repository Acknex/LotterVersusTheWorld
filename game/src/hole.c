

action enemy_hole()
{
	set(me, POLYGON | PASSABLE | FLAG1);
	my.type = TypeHole;
	my.material = ObjectMaterial;
	my.skill41 = floatv(26);
	
	while(1)
	{
		my.skill41 = floatv(0.5 + 0.5 * sinv(total_ticks));
		ent_animate(me, "activate", total_ticks * 2, ANM_CYCLE);
		MARKER_update(me);
		wait(1);
	}
}
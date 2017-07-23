#ifndef UTIL_H
#define UTIL_H

var vec_dist2d(VECTOR * a, VECTOR * b)
{
	return vec_dist(vector(a.x, a.y, 0), vector(b.x, b.y, 0));
}

#endif
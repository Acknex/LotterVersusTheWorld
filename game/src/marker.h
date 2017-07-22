#ifndef MARKER_H
#define MARKER_H

void MARKER_toggle();
void MARKER_attach(ENTITY* ent);
void MARKER_detach(ENTITY* ent);
void MARKER_update(ENTITY* ent);

#include "marker.c"

#endif
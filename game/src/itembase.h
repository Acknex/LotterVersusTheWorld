#ifndef ITEMBASE_H
#define ITEMBASE_H

void ITEM_setup();
void ITEM_fade();
void ITEM_collect();
void ITEM_update();

#define is_collected  FLAG4


#include "itembase.c"

#endif
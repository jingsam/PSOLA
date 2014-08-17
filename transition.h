#ifndef TRANSITION_H
#define TRANSITION_H

#include "planmap.h"

int transition(Cell* mycell);

int classify_land_use(Cell* mycell);

void set_cell_type(Cell* mycell);

#endif
#ifndef RULE_H
#define RULE_H

#include "planmap.h" // Cell
#include "random.h"  // Random

int neighbors_count(Cell* cell, int value, int radius);

bool core_edge_operator(Cell* cell, double p);

void neighbor_effects(Cell* cell, int level);

#endif

#ifndef RULE_H
#define RULE_H

#include "planmap.h" // Cell
#include "random.h"  // Random

int neighbors_has(Cell* cell, int value, int radius);

bool core_edge_operator(Cell* cell, double p);

std::vector<double> neighbor_effects(Cell* cell, int level);

int roulette_wheel(Cell* cell, Random* rnd);

#endif

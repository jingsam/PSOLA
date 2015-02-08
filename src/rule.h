#ifndef RULE_H
#define RULE_H

#include "planmap.h"

bool rule_neighbors_has(Cell* cell, int radius, int value);

bool rule_core_edge(Cell* cell, double p);

#endif

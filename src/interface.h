#ifndef INTERFACE_H
#define INTERFACE_H

#include "datamap.h"    // Map<T>
#include "planmap.h"    // PlanMap, Cell


Map<int> init_map();

void statistics(PlanMap* plan_map);

int transition(Cell* cell);

#endif // INTERFACE_H

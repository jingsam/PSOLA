#ifndef STATS_H
#define STATS_H

#include <map>
#include "datamap.h"    // Map<T>

// proximity
double PROX_MN(Map<int>& datamap, int value);

// core aera index
double CAI_MN(Map<int>& datamap, int value, int depth);

double change_ratio(Map<int>& datamap, Map<int>& land_use_map);

double GDP_MN(Map<int>& datamap, std::map<int, double>& benefits);

#endif

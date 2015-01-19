#ifndef PARAMETER_H
#define PARAMETER_H

#include "tool.h"   // Random
#include "datamap.h"    // Map<T>

extern Random* g_RND;
extern Map<int> g_land_use_map;
extern Map<double> g_arable_suit_map;
extern Map<double> g_orchard_suit_map;
extern Map<double> g_forest_suit_map;
extern Map<double> g_construction_suit_map;
extern Map<double> g_slope_map;
extern Map<double> g_road_map;


void set_parameter();

#endif // PARAMETER_H

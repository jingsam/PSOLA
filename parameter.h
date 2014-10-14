#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include "pso.h"


// core cell and edge cell
extern double g_core;
extern double g_edge;
extern int g_edge_depth;

// quantities of land-use
extern int g_arable;
extern int g_orchard;
extern int g_forest;
extern int g_urban;
extern int g_rural;

// weights of objectives
extern double g_weight_suit;
extern double g_weight_prox;
extern double g_weight_unchange;

// gis data
extern Map<double> g_arable_suit_map;
extern Map<double> g_orchard_suit_map;
extern Map<double> g_forest_suit_map;
extern Map<double> g_construction_suit_map;
extern Map<double> g_slope_map;
extern Map<double> g_road_map;
extern Map<double> g_soil_depth_map;

void set_parameter(const std::string& opt, const std::string& arg);

#endif // PARAMETER_H

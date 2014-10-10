#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>
#include "random.h"
#include "datamap.h"


extern int g_size;                              // number of particles
extern int g_max;                               // max value
extern int g_seed;                              // general random seed
extern Random* g_RND;                           // general random generator
extern double g_momentum;                       // cofficient of keep current speed
extern double g_c1;                             // cofficient of move to Pbest
extern double g_c2;                             // cofficient of move to Gbest
extern int g_r1;                                // random seed 1 of PSO
extern int g_r2;                                // random seed 2 of PSO
extern int g_generation;                        // total number of iterations
extern int g_interval;                          // frequency of output
extern std::string g_output;                    // output directoy
extern std::string g_land_use;                  // land use map
extern int g_xsize;                             // columns
extern int g_ysize;                             // rows
extern int g_nodata;                            // nodata

// core cell and edge cell
extern double g_core;
extern double g_edge;
extern int g_edge_depth;

// quantities of land-use
extern int g_arable;
extern int g_orchard;
extern int g_forest;
extern int g_construction;

// weights of objectives
extern double g_weight_suit;
extern double g_weight_prox;
extern double g_weight_unchange;

// gis data
extern Map<int>    g_land_use_map;              // land use map
extern Map<double> g_arable_suit_map;           // suitability map of arableland
extern Map<double> g_orchard_suit_map;          // suitability map of orchard
extern Map<double> g_forest_suit_map;           // suitability map of forest
extern Map<double> g_construction_suit_map;     // suitability map of construction
extern Map<double> g_slope_map;                 // slope map
extern Map<double> g_road_map;                  // road map
extern Map<double> g_soil_depth_map;            // soil depth map

int  parse_option(const int argc, const char * const argv[]);
void show_option();

#endif // OPTION_H

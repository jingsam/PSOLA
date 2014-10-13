#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>
#include "random.h"
#include "datamap.h"


extern int g_size;                              // number of particles
extern int g_max;                               // max value
extern int g_seed;                              // general random seed

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

extern Random* g_RND;
extern Map<int> g_land_use_map;

int parse_option(const int argc, const char * const argv[]);
void show_option();

#endif // OPTION_H

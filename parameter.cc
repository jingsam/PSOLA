#include "parameter.h"
#include "raster.h"

double g_core = 1.0;
double g_edge = 1.0;
int g_edge_depth = 1;

int g_arable;
int g_orchard;
int g_forest;
int g_construction;

double g_weight_suit;
double g_weight_prox;
double g_weight_unchange;

Map<double> g_arable_suit_map;
Map<double> g_orchard_suit_map;
Map<double> g_forest_suit_map;
Map<double> g_grass_suit_map;
Map<double> g_construction_suit_map;
Map<double> g_slope_map;
Map<double> g_road_map;
Map<double> g_soil_depth_map;


template <typename T>
void set_map(const std::string& filename, Map<T>& map) {
    int xsize = getRasterXSize( filename.c_str() );
    int ysize = getRasterYSize( filename.c_str() );
    readRaster( map, filename.c_str() );
    map.xsize = xsize;
    map.ysize = ysize;
}

void set_core(const std::string& arg) {
    g_core = std::atof( arg.c_str() );
}

void set_edge(const std::string& arg) {
    g_edge = std::atof( arg.c_str() );
}

void set_edge_depth(const std::string& arg) {
    g_edge_depth = std::atoi( arg.c_str() );
}

void set_arable(const std::string& arg) {
    g_arable = std::atoi( arg.c_str() );
}

void set_orchard(const std::string& arg) {
    g_orchard = std::atoi( arg.c_str() );
}

void set_forest(const std::string& arg) {
    g_forest = std::atoi( arg.c_str() );
}

void set_construction(const std::string& arg) {
    g_construction = std::atoi( arg.c_str() );
}

void set_weight_suit(const std::string& arg) {
    g_weight_suit = std::atof( arg.c_str() );
}

void set_weight_prox(const std::string& arg) {
    g_weight_prox = std::atof( arg.c_str() );
}

void set_weight_unchange(const std::string& arg) {
    g_weight_unchange = std::atof( arg.c_str() );
}

void set_arable_suit_map(const std::string& arg) {
    set_map( arg, g_arable_suit_map );
}

void set_orchard_suit_map(const std::string& arg) {
    set_map( arg, g_orchard_suit_map );
}

void set_forest_suit_map(const std::string& arg) {
    set_map( arg, g_forest_suit_map );
}

void set_construction_suit_map(const std::string& arg) {
    set_map( arg, g_construction_suit_map );
}

void set_slope_map(const std::string& arg) {
    set_map( arg, g_slope_map );
}

void set_road_map(const std::string& arg) {
    set_map( arg, g_road_map );
}

void set_soil_depth_map(const std::string& arg) {
    set_map( arg, g_soil_depth_map );
}

void set_parameter(const std::string& opt, const std::string& arg)
{
    if      (opt == "core")                     set_core(arg);
    else if (opt == "edge")                     set_edge(arg);
    else if (opt == "edge-depth")               set_edge_depth(arg);

    else if (opt == "arable")                   set_arable(arg);
    else if (opt == "orchard")                  set_orchard(arg);
    else if (opt == "forest")                   set_forest(arg);
    else if (opt == "construction")             set_construction(arg);

    else if (opt == "weight-suit")              set_weight_suit(arg);
    else if (opt == "weight-prox")              set_weight_prox(arg);
    else if (opt == "weight-unchange")          set_weight_unchange(arg);

    else if (opt == "arable_suit_map")          set_arable_suit_map(arg);
    else if (opt == "orchard_suit_map")         set_orchard_suit_map(arg);
    else if (opt == "forest_suit_map")          set_forest_suit_map(arg);
    else if (opt == "construction_suit_map")    set_construction_suit_map(arg);
    else if (opt == "slope-map")                set_slope_map(arg);
    else if (opt == "road-map")                 set_road_map(arg);
    else if (opt == "soil-depth-map")           set_soil_depth_map(arg);
}

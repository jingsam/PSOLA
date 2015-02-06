#include "parameter.h"


Map<double> g_arable_suit_map;
Map<double> g_orchard_suit_map;
Map<double> g_forest_suit_map;
Map<double> g_construction_suit_map;
Map<double> g_slope_map;
Map<double> g_road_map;

void set_parameter()
{
    readRaster(g_arable_suit_map, g_option["arable-suit-map"].c_str());
    readRaster(g_orchard_suit_map, g_option["orchard-suit-map"].c_str());
    readRaster(g_forest_suit_map, g_option["forest-suit-map"].c_str());
    readRaster(g_construction_suit_map, g_option["construction-suit-map"].c_str());
    readRaster(g_slope_map, g_option["slope-map"].c_str());
    readRaster(g_road_map, g_option["road-map"].c_str());
}

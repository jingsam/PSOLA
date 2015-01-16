#include "parameter.h"
#include "raster.h" // readRaster()

Random* g_RND;
Map<int> g_land_use_map;
Map<double> g_arable_suit_map;
Map<double> g_orchard_suit_map;
Map<double> g_forest_suit_map;
Map<double> g_construction_suit_map;
Map<double> g_slope_map;
Map<double> g_road_map;

void set_parameter(std::map<std::string, std::string>& option)
{
    g_RND = new Random(std::stoi(option["seed"]));
    readRaster(g_land_use_map, option["land-use-map"].c_str());
    readRaster(g_arable_suit_map, option["arable-suit-map"].c_str());
    readRaster(g_orchard_suit_map, option["orchard-suit-map"].c_str());
    readRaster(g_forest_suit_map, option["forest-suit-map"].c_str());
    readRaster(g_construction_suit_map, option["construction-suit-map"].c_str());
    readRaster(g_slope_map, option["slope-map"].c_str());
    readRaster(g_road_map, option["road-map"].c_str());
}

#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>
#include "random.h"
#include "datamap.h"

extern Random* g_RND;                           // general random generator
extern int g_size;                              // number of particles
extern int g_max;                               // max value
extern int g_seed;                              // general random seed
extern double g_momentum;                       // cofficient of keep current speed
extern double g_c1;                             // cofficient of move to Pbest
extern double g_c2;                             // cofficient of move to Gbest
extern int g_r1;                                // random seed 1 of PSO
extern int g_r2;                                // random seed 2 of PSO
extern int g_generation;                        // total number of iterations
extern std::string g_output;                    // output directoy
extern int g_interval;                          // frequency of output

extern double g_core;
extern double g_edge;
extern int g_edge_depth;
extern int g_num_arable;
extern int g_num_urban;
extern Map<double> g_soil_depth_map;

extern std::string g_region;                    // 优化区域
extern int g_xsize;                             // 地图的宽度（列数）
extern int g_ysize;                             // 地图的高度（行数）
extern int g_nodata;                            // nodata值
extern Map<int> g_region_map;                   // 优化区域图
extern Map<int> g_land_use_map;                 // 土地利用现状图
extern Map<double> g_arable_suit_map;           // 耕地适宜性图
extern Map<double> g_orchard_suit_map;          // 园地适宜性图
extern Map<double> g_forest_suit_map;           // 林地适宜性图
extern Map<double> g_grass_suit_map;            // 草地适宜性图
extern Map<double> g_urban_suit_map;            // 城镇用地适宜性图
extern Map<double> g_rural_suit_map;            // 农村用地适宜性图
extern Map<double> g_slope_map;                 // 坡度图
extern Map<double> g_road_map;                  // 道路图
extern std::vector<int> g_land_use_struct;      // 土地利用结构
extern std::vector<double> g_obj_weights;       // 目标函数权重

void show_help();
void show_version();
void show_error(const char * const msg);
int  parse_option(const int argc, const char * const argv[]);
void set_option(const std::string& opt, const std::string& arg);
void show_option();

void set_size(const std::string& arg);
void set_max(const std::string& arg);
void set_seed(const std::string& arg);
void set_momentum(const std::string& arg);
void set_c1(const std::string& arg);
void set_c2(const std::string& arg);
void set_r1(const std::string& arg);
void set_r2(const std::string& arg);
void set_generation(const std::string& arg);
void set_output(const std::string& arg);
void set_interval(const std::string& arg);

void set_region_map(const std::string& arg);
void set_land_use_map(const std::string& arg);
void set_arable_suit_map(const std::string& arg);
void set_orchard_suit_map(const std::string& arg);
void set_forest_suit_map(const std::string& arg);
void set_grass_suit_map(const std::string& arg);
void set_urban_suit_map(const std::string& arg);
void set_rural_suit_map(const std::string& arg);
void set_slope_map(const std::string& arg);
void set_road_map(const std::string& arg);
void set_land_use_struct(const std::string& arg);
void set_obj_weights(const std::string& arg);

void parse_xml_file(const std::string& arg);


#endif // OPTION_H

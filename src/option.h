#ifndef OPTION_H
#define OPTION_H

#include <map>
#include <string>
#include "datamap.h"    // Map<T>
#include "random.h"     // Random

extern std::map<std::string, std::string> g_option;
extern Random* g_rnd;
extern Map<int> g_land_use_map;

void parse_option(const int argc, const char * const argv[]);
void show_option();

#endif // OPTION_H

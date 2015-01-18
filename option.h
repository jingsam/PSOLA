#ifndef OPTION_H
#define OPTION_H

#include <map>
#include <string>
#include "datamap.h"    // Map<T>

extern std::map<std::string, std::string> g_option;


void parse_option(const int argc, const char * const argv[]);
void show_option();

#endif // OPTION_H

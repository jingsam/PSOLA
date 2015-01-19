#ifndef TOOL_H
#define TOOL_H

#include <string>
#include "random.h" // Random



void normalize(std::vector<double>& p);

// return (i + 1)
int roulette_wheel(std::vector<double>& p, Random* rnd);

int stoi(const std::string& str);

double stod(const std::string& str);

std::string to_string(int i);

std::string to_string(double d);

#endif

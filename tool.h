#ifndef TOOL_H
#define TOOL_H

#include "random.h"

void normalize(std::vector<double>& p);

// return (i + 1)
int roulette_wheel(std::vector<double>& p, Random* rnd);


#endif

#ifndef TOOL_H
#define TOOL_H

#include "random.h"

void normalize_p(std::vector<double>& p);

// return (i + 1)
int roulette_heel(std::vector<double>& p, Random* rnd);


#endif

#ifndef FITNESS_H
#define FITNESS_H

#include "datamap.h"

double calc_fitness(Map<int>& scheme);

double suitability(Map<int>& scheme);

double proximity(Map<int>& scheme);

#endif // FITNESS_H

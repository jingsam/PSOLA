#ifndef PSOLA_H
#define PSOLA_H

#include "arg/arg_parser.h"
#include "xml/tinyxml2.h"
#include "datamap.h"
#include "planmap.h"
#include "pso.h"
#include "init.h"
#include "raster.h"
#include "option.h"
#include "tool.h"
#include "stats.h"
#include "rule.h"

// functions must be implemented by users
Map<int> init_map();
void fitness(PlanMap* plan_map);
int transition(Cell* cell);

#endif

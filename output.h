#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include "pso.h"
#include "xml/tinyxml2.h"


void init_output();

tinyxml2::XMLDocument* createLogDocument();

void logStatus(tinyxml2::XMLDocument* doc, 
    int generation, double fitness, const char *filename = NULL);

int outputImage(const std::vector<int>& data, const char *filename);

#endif

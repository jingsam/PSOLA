#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <string>


void normalize(std::vector<double>& p);

int stoi(const std::string& str);

double stod(const std::string& str);

std::string to_string(int i);

std::string to_string(double d);

#endif

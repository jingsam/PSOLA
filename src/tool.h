#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <string>


void normalize(std::vector<double>& p);

void normalize(std::vector<double> &p1, std::vector<double> &p2);

int routtle_wheel(std::vector<double> p, double r);

int stoi(const std::string& str);

double stod(const std::string& str);

std::string to_string(int i);

std::string to_string(double d);

#endif

#include "tool.h"
#include <cstdlib>  // atoi(), atof()
#include <string>
#include <sstream>  // ostringstream

void normalize(std::vector<double> &p)
{
    double sum = 0.0;
    for (int i = 0; i < p.size(); ++i) {
        sum += p[i];
    }

    if (sum == 0.0) return;

    for (int j = 0; j < p.size(); ++j) {
        p[j] /= sum;
    }
}

int stoi(const std::string& str)
{
    return atoi(str.c_str());
}

double stod(const std::string& str)
{
    return atof(str.c_str());
}

std::string to_string(int i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

std::string to_string(double d)
{
    std::ostringstream oss;
    oss << d;
    return oss.str();
}

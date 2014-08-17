#include "tool.h"

void normalize_p(std::vector<double> &p)
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

int roulette_wheel(std::vector<double> &p, Random* rnd)
{
    double r = rnd->nextDouble();
    double sum = 0.0;
    for (int i = 0; i < p.size(); ++i) {
        sum += p[i];
        if ( sum >= r && p[i] != 0.0 ) return i + 1;
    }

    return rnd->nextInt( p.size() ) + 1;
}

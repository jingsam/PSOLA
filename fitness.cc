#include <cstdlib>
#include <cassert>

#include "fitness.h"
#include "option.h"
#include "datamap.h"
#include "tool.h"


double calc_fitness(Map<int>& scheme)
{
    double suit = suitability(scheme);
    double proxi = proximity(scheme);

    return g_obj_weights[0] * suit +
            g_obj_weights[1] * proxi;
}

double suitability(Map<int>& scheme)
{
    int count = 0;
    double sum = 0.0;

    for (int i = 0; i < scheme.size(); ++i) {
        int value = scheme.at(i);
        if (value == g_nodata) continue;

        switch (value) {
            case 1: sum += g_arable_suit_map.at(i);   count++;    break;
            case 2: sum += g_orchard_suit_map.at(i);  count++;    break;
            case 3: sum += g_forest_suit_map.at(i);   count++;    break;
            case 4: sum += g_grass_suit_map.at(i);    count++;    break;
            case 5: sum += g_urban_suit_map.at(i);    count++;    break;
            case 6: sum += g_rural_suit_map.at(i);    count++;    break;
            default : break;
        }
    }

    assert(count != 0);
    return sum / count;
}

double proximity(Map<int>& scheme)
{
    int count = 0;
    double sum = 0.0;

    for (int y = 0; y < scheme.ysize; ++y) {
        for (int x = 0; x < scheme.xsize; ++x) {
            int value = scheme.atxy(x, y);
            if (value == g_nodata) continue;

            int cnt = 0;
            std::vector<int> neighbors = scheme.neighbors(x, y, 1);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) == value)
                { cnt++; }
            }

            if (neighbors.size() == 0) { sum += 1.0; }
            else { sum += (double)cnt / neighbors.size(); }

            count++;
        }
    }

    assert(count != 0);
    return sum / count;
}

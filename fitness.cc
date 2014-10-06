#include <cstdlib>
#include "fitness.h"
#include "option.h"
#include "datamap.h"
#include "tool.h"

double calc_fitness(Map<int>& scheme);
double suitability(Map<int>& scheme);
double proximity(Map<int>& scheme);
double unchange_ratio(Map<int>& scheme);

double calc_fitness(Map<int>& scheme)
{
    double suit = suitability(scheme);
    double prox = proximity(scheme);
    double unchange = unchange_ratio(scheme);

    return g_weight_suit * suit +
            g_weight_prox * prox +
            g_weight_unchange * unchange;
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
            case 5:
            case 6: sum += g_build_suit_map.at(i);    count++;    break;
        }
    }

    return sum / count;
}

double proximity(Map<int>& scheme)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < scheme.ysize; ++y) {
        for (int x = 0; x < scheme.xsize; ++x) {
            int value = scheme.atxy(x, y);
            if (value == g_nodata) continue;

            int cnt = 0;
            std::vector<int> neighbors = scheme.neighbors(x, y, 1);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) == value) cnt++;
            }

            sum += (double)cnt / neighbors.size();
            count++;
        }
    }

    return sum / count;
}

double unchange_ratio(Map<int>& scheme)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < scheme.size(); ++i) {
        int value = scheme.at(i);
        if (value == g_nodata) continue;

        int land_use = g_land_use_map.at(i);
        if (value == land_use) sum++;
        count++;
    }

    return (double)sum / count;
}

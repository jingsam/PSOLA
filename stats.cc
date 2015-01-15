#include <cstdlib>
#include <cassert>
#include <iostream>
#include "stats.h"


double fitness(Map<int>& plan_map);
double compactness(Map<int>& plan_map);
double cost(Map<int>& plan_map);
double core_area_index(Map<int>& plan_map);

std::map<std::string, double> statistics(Map<int>& plan_map)
{
    std::map<std::string, double> stats;

    stats["fitness"] = fitness(plan_map);
    stats["social-benefit"] = compactness(plan_map);
    stats["economic-benefit"] = cost(plan_map);
    stats["ecological-benefit"] = core_area_index(plan_map);

    return stats;
}

double fitness(Map<int>& plan_map)
{
    double suit = suitability(plan_map);
    double prox = proximity(plan_map);
    double unchange = unchange_ratio(plan_map);

    return g_weight_suit * suit +
            g_weight_prox * prox +
            g_weight_unchange * unchange;
}

double compactness(Map<int>& plan_map)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < plan_map.ysize; ++y) {
        for (int x = 0; x < plan_map.xsize; ++x) {
            int value = plan_map.atxy(x, y);
            if (value == g_nodata) continue;

            int cnt = 0;
            std::vector<int> neighbors = plan_map.neighbors(x, y, 1);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) == value) cnt++;
            }

            assert(neighbors.size() != 0);
            sum += (double)cnt / neighbors.size();
            count++;
        }
    }

    assert(count != 0);
    return sum / count;
}

double cost(Map<int>& plan_map)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < plan_map.size(); ++i) {
        int value = plan_map.at(i);
        if (value == g_nodata) continue;

        int land_use = g_land_use_map.at(i);
        if (value == land_use) sum++;
        count++;
    }

    assert(count != 0);
    return (double)sum / count;
}

double core_area_index(Map<int>& plan_map)
{

}

double suitability(Map<int>& plan_map)
{
    int count = 0;
    double sum = 0.0;

    for (int i = 0; i < plan_map.size(); ++i) {
        int value = plan_map.at(i);
        if (value == g_nodata) continue;

        switch (value) {
            case 1: sum += g_arable_suit_map.at(i);       count++; break;
            case 2: sum += g_orchard_suit_map.at(i);      count++; break;
            case 3: sum += g_forest_suit_map.at(i);       count++; break;
            case 5:
            case 6: sum += g_construction_suit_map.at(i); count++; break;
        }
    }

    assert(count != 0);
    return sum / count;
}

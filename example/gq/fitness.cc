#include "../../src/psola.h"
#include "parameter.h"
#include <cstdlib>   // abs()

double PROX_MN_rural(Map<int>& datamap);
double suit(Map<int>& datamap);
double urban(Map<int>& datamap);

void fitness(PlanMap* plan_map)
{
    double w1 = stod(g_option["social-benefit"]);
    double w2 = stod(g_option["economic-benefit"]);
    double w3 = stod(g_option["ecological-benefit"]);

    Map<int> datamap = plan_map->getDataMap();
    double o1 = PROX_MN_rural(datamap);
    double o2 = urban(datamap);
    double o3 = suit(datamap);
    double fitness = w1 * o1 + w2 * o2 + w3 *o3;

    plan_map->stats["social-benefit"] = o1;
    plan_map->stats["economic-benefit"] = o2;
    plan_map->stats["ecological-benefit"] = o3;
    plan_map->stats["fitness"] = fitness;
}

double PROX_MN_rural(Map<int>& datamap)
{
    std::vector<int> values;
    values.push_back(5);

    return PROX_MN(datamap, values);
}

double urban(Map<int>& datamap)
{
    int max = stoi(g_option["urban"]);
    int count = 0;

    for (int i = 0; i < datamap.size(); ++i)
    {
        int value = datamap.at(i);
        if (value == 4) count++;
    }

    return max > count ? (double)count / max : (double)max / count;
}

double suit(Map<int>& datamap)
{
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < datamap.size(); ++i)
    {
        int value = datamap.at(i);
        if (value == datamap.nodata) continue;

        switch (value) {
            case 0: sum += g_arable_suit_map.at(i); count++; break;
            case 1: sum += g_orchard_suit_map.at(i); count++; break;
            case 2: sum += g_forest_suit_map.at(i); count++; break;
            case 4:
            case 5: sum += g_construction_suit_map.at(i); count++; break;
        }
    }

    return count != 0 ? sum / count : 0.0;
}

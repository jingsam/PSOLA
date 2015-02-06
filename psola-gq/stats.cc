#include "stats.h"
#include "parameter.h"


double compactness(Map<int>& plan_map);
double cost(Map<int>& plan_map);
double core_area_index(Map<int>& plan_map);
double suit(Map<int>& datamap);
double GDP(Map<int>& datamap);


void statistics(PlanMap* plan_map)
{
    double w1 = stod(g_option["social-benefit"]);
    double w2 = stod(g_option["economic-benefit"]);
    double w3 = stod(g_option["ecological-benefit"]);

    Map<int> datamap = plan_map->getDataMap();
    double o1 = compactness(datamap);
    double o2 = cost(datamap);
    double o3 = suit(datamap);
    double fitness = w1 * o1 + w2 * o2 + w3 *o3;

    plan_map->stats["social-benefit"] = o1;
    plan_map->stats["economic-benefit"] = o2;
    plan_map->stats["ecological-benefit"] = o3;
    plan_map->stats["fitness"] = fitness;
}

double compactness(Map<int>& datamap)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            int value = datamap.atxy(x, y);
            if (value == datamap.nodata) continue;

            int cnt = 0;
            std::vector<int> neighbors = datamap.neighbors(x, y, 1);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) == value) cnt++;
            }

            if (neighbors.size() != 0) {
                sum += (double)cnt / neighbors.size();
            }
            count++;
        }
    }

    return count != 0 ? sum / count : 0.0;
}

double cost(Map<int>& datamap)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < datamap.size(); ++i) {
        int value = datamap.at(i);
        if (value == datamap.nodata) continue;

        int land_use = g_land_use_map.at(i);
        if (value == land_use) sum++;
        count++;
    }

    return count != 0 ? (double)sum / count : 0.0;
}

double core_area_index(Map<int>& datamap)
{
    int sum = 0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            int value = datamap.atxy(x, y);
            if (value == datamap.nodata) continue;
            if (value != 3) continue;

            std::vector<int> neighbors = datamap.neighbors(x, y, 4);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) != value) sum++;
                break;
            }
            count++;
        }
    }

    return count != 0 ? 1 - (double)sum / count : 0.0;
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
            case 1: sum += g_arable_suit_map.at(i); count++; break;
            case 2: sum += g_orchard_suit_map.at(i); count++; break;
            case 3: sum += g_forest_suit_map.at(i); count++; break;
            case 5:
            case 6: sum += g_construction_suit_map.at(i); count++; break;
        }
    }

    return count != 0 ? sum / count : 0.0;
}

double GDP(Map<int>& datamap)
{
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < datamap.size(); ++i)
    {
        int value = datamap.at(i);
        if (value == datamap.nodata) continue;

        switch (value) {
            case 1: sum += 0.05; count++; break;
            case 2: sum += 0.14; count++; break;
            case 3: sum += 0.13; count++; break;
            case 5: sum += 1.00; count++; break;
            case 6: sum += 0.00; count++; break;
        }
    }

    return count != 0 ? sum / count : 0.0;
}

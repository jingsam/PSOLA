#include "stats.h"
#include <string>       // stod()
#include "datamap.h"    // Map<T>
#include "option.h"     // g_option
#include "parameter.h"  // g_land_use_map


double compactness(Map<int>& plan_map);
double cost(Map<int>& plan_map);
double core_area_index(Map<int>& plan_map);

void statistics(PlanMap* plan_map)
{
    double w1 = std::stod(g_option["social-benefit"]);
    double w2 = std::stod(g_option["economic-benefit"]);
    double w3 = std::stod(g_option["ecological-benefit"]);

    Map<int> datamap = plan_map.getDataMap();
    double o1 = compactness(plan_map);
    double o2 = cost(plan_map);
    double o3 = core_area_index(plan_map);
    double fitness = w1 * o1 + w2 * o2 + w3 *o3;

    plan_map->stats["social-benefit"] = o1;
    plan_map->stats["economic-benefit"] = o2;
    plan_map->stats["ecological-benefit"] = o3;
    plan_map->stats["fitness"] = fitness;
}

double compactness(Map<int>& plan_map)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < plan_map.ysize; ++y) {
        for (int x = 0; x < plan_map.xsize; ++x) {
            int value = plan_map.atxy(x, y);
            if (value == plan_map.nodata) continue;

            int cnt = 0;
            std::vector<int> neighbors = plan_map.neighbors(x, y, 1);
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

double cost(Map<int>& plan_map)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < plan_map.size(); ++i) {
        int value = plan_map.at(i);
        if (value == plan_map.nodata) continue;

        int land_use = g_land_use_map.at(i);
        if (value == land_use) sum++;
        count++;
    }

    return count != 0 ? (double)sum / count : 0.0;
}

double core_area_index(Map<int>& plan_map)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < plan_map.size(); ++i) {
        int value = plan_map.at(i);
        if (value == plan_map.nodata) continue;
        if (value != 3) continue;

        std::vector<int> neighbors = plan_map.neighbors(x, y, 4);
        for (int i = 0; i < neighbors.size(); ++i) {
            if (neighbors.at(i) != value) sum++;
            break;
        }
        count++;
    }

    return count != 0 ? 1 - (double)sum / count : 0.0;
}

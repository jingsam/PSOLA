#include <algorithm>
#include "stats.h"


double PROX_MN(Map<int>& datamap, int value)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            if (datamap.atxy(x, y) != value) continue;

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

double CAI_MN(Map<int>& datamap, int value, int depth)
{
    int sum = 0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            if (datamap.atxy(x, y) != value) continue;

            std::vector<int> neighbors = datamap.neighbors(x, y, depth);
            for (int i = 0; i < neighbors.size(); ++i) {
                if (neighbors.at(i) != value) sum++;
                break;
            }
            count++;
        }
    }

    return count != 0 ? 1 - (double)sum / count : 0.0;
}

double change_ratio(Map<int>& datamap, Map<int>& land_use_map)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < datamap.size(); ++i) {
        int value = datamap.at(i);
        if (value == datamap.nodata) continue;

        int land_use = land_use_map.at(i);
        if (value == land_use) sum++;
        count++;
    }

    return count != 0 ? (double)sum / count : 0.0;
}

double GDP(Map<int>& datamap, std::map<int, double>& benefits)
{
    double sum = 0.0;

    for (int i = 0; i < datamap.size(); ++i)
    {
        int value = datamap.at(i);
        if (value == datamap.nodata) continue;

        sum += benefits[value];
    }

    return sum;
}

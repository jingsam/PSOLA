#include <algorithm>
#include "stats.h"


double PROX_MN(Map<int>& datamap, std::vector<int>& values)
{
    double sum = 0.0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            int value = datamap.atxy(x, y);
            if (value == datamap.nodata) continue;

            std::vector<int>::iterator iter;
            iter = std::find(values.begin(), values.end(), value);
            if (iter == values.end()) continue;

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

double CAI_MN(Map<int>& datamap, std::vector<int>& values)
{
    int sum = 0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            int value = datamap.atxy(x, y);
            if (value == datamap.nodata) continue;

            std::vector<int>::iterator iter;
            iter = std::find(values.begin(), values.end(), value);
            if (iter == values.end()) continue;

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

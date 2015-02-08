#include "objective.h"


double PROX_MN(Map<int>& datamap)
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

double CAI_MN(Map<int>& datamap)
{
    int sum = 0;
    int count = 0;

    for (int y = 0; y < datamap.ysize; ++y) {
        for (int x = 0; x < datamap.xsize; ++x) {
            int value = datamap.atxy(x, y);
            if (value == datamap.nodata) continue;

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

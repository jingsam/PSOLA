#ifndef DATAMAP_H
#define DATAMAP_H

#include <cstdlib>
#include <vector>

template<class T>
class Map : public std::vector<T>
{
public:
    Map() {}

    Map(int xsize, int ysize, double nodata, const T& value) : std::vector<T>(xsize * ysize, value) {
        this->xsize = xsize;
        this->ysize = ysize;
        this->nodata = nodata
    }

    virtual ~Map() {}

    T& atxy(int x, int y) {
        return this->at(x + y * xsize);
    }

    const T& atxy(int x, int y) const {
        return this->at(x + y * xsize);
    }

    std::vector<T> neighbors(int x, int y, int level) {
        std::vector<T> neighbors;
        for (int j = y - level; j <= y + level; ++j) {
            if (j < 0 || j >= ysize) continue;

            for (int i = x - level; i <= x + level; ++i) {
                if (i < 0 || i >= xsize) continue;
                if (i == x && j == y) continue;

                neighbors.push_back(this->atxy(i,j));
            }
        }

        return neighbors;
    }

    std::vector<T> neighbors4(int x, int y, int level) {
        std::vector<T> neighbors;
        for (int j = y - level; j <= y + level; ++j) {
            if (j < 0 || j >= ysize) continue;

            for (int i = x - level; i <= x + level; ++i) {
                if (i < 0 || i >= xsize) continue;
                if (i == x && j == y) continue;

                int dx = abs(i - x);
                int dy = abs(j - y);
                if ((dx + dy) <= level) {
                    neighbors.push_back(this->atxy(i,j));
                }
            }
        }

        return neighbors;
    }


    int xsize, ysize;
    double nodata;
};

#endif

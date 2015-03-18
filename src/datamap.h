#ifndef DATAMAP_H
#define DATAMAP_H

#include <cstdlib> // abs()
#include <vector>


template<class T>
class Map : public std::vector<T>
{
public:
    Map() {}

    Map(int xsize, int ysize, const T& value, double nodata)
    : std::vector<T>(xsize * ysize, value) {
        this->xsize = xsize;
        this->ysize = ysize;
        this->nodata = nodata;
    }

    Map(const Map<T>& other) : std::vector<T>(other) {
        this->xsize = other.xsize;
        this->ysize = other.ysize;
        this->nodata = other.nodata;
    }

    virtual ~Map() {}

    Map& operator=(const Map<T>& other) {
        this->std::vector<T>::operator=(other);
        this->xsize = other.xsize;
        this->ysize = other.ysize;
        this->nodata = other.nodata;

        return *this;
    }

    T& atxy(int x, int y) {
        return this->at(x + y * xsize);
    }

    const T& atxy(int x, int y) const {
        return this->at(x + y * xsize);
    }

    std::vector<T> neighbors(int x, int y, int radius) {
        std::vector<T> neighbors;
        for (int j = y - radius; j <= y + radius; ++j) {
            if (j < 0 || j >= ysize) continue;

            for (int i = x - radius; i <= x + radius; ++i) {
                if (i < 0 || i >= xsize) continue;
                if (i == x && j == y) continue;

                neighbors.push_back(this->atxy(i,j));
            }
        }

        return neighbors;
    }

    std::vector<T> neighbors4(int x, int y, int radius) {
        std::vector<T> neighbors;
        for (int j = y - radius; j <= y + radius; ++j) {
            if (j < 0 || j >= ysize) continue;

            for (int i = x - radius; i <= x + radius; ++i) {
                if (i < 0 || i >= xsize) continue;
                if (i == x && j == y) continue;

                int dx = abs(i - x);
                int dy = abs(j - y);
                if ((dx + dy) <= radius) {
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

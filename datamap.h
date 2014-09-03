#ifndef DATAMAP_H
#define DATAMAP_H

#include <cstdlib>
#include "gdal_priv.h"
#include "cpl_conv.h"


template<class T>
class Map : public std::vector<T>
{
public:
    Map() {}

    Map(int xsize, int ysize, const T& value) : std::vector<T>(xsize * ysize, value) {
        _xsize = xsize;
        _ysize = ysize;
    }

    virtual ~Map() {}

    T& at(int x, int y) {
        return this->at(x + y * _xsize);
    }

    const T& at(int x, int y) const {
        return this->at(x + y * _xsize);
    }
    
    int xsize() {
        return _xsize;
    }
    
    int ysize() {
        return _ysize;
    }

    std::vector<T> neighbors(int x, int y, int level) {  
        std::vector<T> neighbors;
        for (int j = y - level; j <= y + level; ++j) {
            if (j < 0 || j > _ysize) continue;

            for (int i = x - level; i <= x + level; ++i) {
                if (i < 0 || i > _xsize) continue;
                if (i == x && j == y) continue;

                neighbors.push_back( this->at(i,j) );
            }
        }

        return neighbors; 
    }

    std::vector<T> neighbors4(int x, int y, int level) {
        std::vector<T> neighbors;
        for (int j = y - level; j <= y + level; ++j) {
            if (j < 0 || j > ysize) continue;
    
            for (int i = x - level; i <= x + level; ++i) {
                if (i < 0 || i > xsize) continue;
                if (i == x && j == y) continue;
    
                int dx = abs(i - x);
                int dy = abs(j - y);
                if ( (dx + dy) <= level ) {
                    neighbors.push_back( this->at(i,j) );
                }            
            }
        }
    
        return neighbors;
    }

private:
    int _xsize, _ysize;
};

#endif

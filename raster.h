#ifndef RASTER_H
#define RASTER_H

#include "datamapmap.h" // class Map<T>


int readRaster(Map<int>& datamap, const char *filename);

int readRaster(Map<double>& datamap, const char *filename);

int writeRaster(const Map<int>& datamap, const char *filename);

int writeRaster(const Map<double>& datamap, const char *filename);

int copyRaster(const char *oldfilename, const char *newfilename);

#endif // RASTER_H

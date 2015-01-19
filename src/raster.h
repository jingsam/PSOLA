#ifndef RASTER_H
#define RASTER_H

#include "datamap.h" // Map<T>


int readRaster(Map<int>& datamap, const char *filename);

int readRaster(Map<double>& datamap, const char *filename);

int writeRaster(const Map<int>& datamap, const char *filename);

int writeRaster(const Map<double>& datamap, const char *filename);

int writeRaster(const Map<int>& datamap, const char *filename,
    const char *ref_file);

int copyRaster(const char *oldfilename, const char *newfilename);

#endif // RASTER_H

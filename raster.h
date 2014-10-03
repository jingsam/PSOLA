#ifndef RASTER_H
#define RASTER_H

#include <vector>


int getRasterXSize(const char *filename);

int getRasterYSize(const char *filename);

double getRasterMaximum(const char *filename);

double getRasterMinimum(const char *filename);

double getRasterNoDataValue(const char *filename);

int readRaster(std::vector<int>& data, const char *filename);

int readRaster(std::vector<double>& data, const char *filename);

int writeRaster(const std::vector<int>& data, const char *filename);

int writeRaster(const std::vector<double>& data, const char *filename);

int copyRaster(const char *oldfilename, const char *newfilename);

#endif // RASTER_H

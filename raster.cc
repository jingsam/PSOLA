#include <vector>
#include <algorithm>

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "datamap.h"
#include "raster.h"


int getRasterXSize(const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 0;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nXSize = poBand->GetXSize();

	GDALClose(poDataset);

	return nXSize;
}

int getRasterYSize(const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 0;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nYSize = poBand->GetYSize();

	GDALClose(poDataset);

	return nYSize;
}

double getRasterMaximum(const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 0.0;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	double max = poBand->GetMaximum();

	GDALClose(poDataset);

	return max;	
}

double getRasterMinimum(const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 0.0;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	double min = poBand->GetMinimum();

	GDALClose(poDataset);

	return min;
}

double getRasterNoDataValue(const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 0.0;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nodata = poBand->GetNoDataValue();

	GDALClose(poDataset);

	return nodata;
}

int readRaster(std::vector<int>& data, const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 3;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nXSize = poBand->GetXSize();
	int nYSize = poBand->GetYSize();

	int* pData;
	pData = (int*)CPLMalloc(sizeof(int)*nXSize*nYSize);
	int ret = poBand->RasterIO( GF_Read,
				0, 0, nXSize, nYSize,
				pData, nXSize, nYSize,
				GDT_Int32, 0, 0);

	data.assign(pData, pData + nXSize * nYSize);

	CPLFree(pData);
	GDALClose(poDataset);	

	return ret;
}

int readRaster(std::vector<double>& data, const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
	if ( !poDataset ) return 3;

	GDALRasterBand* poBand;
	poBand = poDataset->GetRasterBand(1);
	int nXSize = poBand->GetXSize();
	int nYSize = poBand->GetYSize();

	double*	pData;
	pData = (double*)CPLMalloc(sizeof(double)*nXSize*nYSize);
	int ret = poBand->RasterIO( GF_Read,
				0, 0, nXSize, nYSize,
				pData, nXSize, nYSize,
				GDT_Float64, 0, 0);

	data.assign(pData, pData + nXSize * nYSize);

	CPLFree(pData);
	GDALClose(poDataset);

	return ret;
}

int writeRaster(const std::vector<int>& data, const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_Update);
	if ( !poDataset ) return 3;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nXSize = poBand->GetXSize();
	int nYSize = poBand->GetYSize();

	int* pData;
	pData = (int*)CPLMalloc(sizeof(int)*nXSize*nYSize);
	std::copy(data.begin(), data.end(), pData);

	int ret = poBand->RasterIO( GF_Write,
				0, 0, nXSize, nYSize,
				pData, nXSize, nYSize,
				GDT_Int32, 0, 0);

	CPLFree(pData);
	GDALClose(poDataset);

	return ret;
}

int writeRaster(const std::vector<double>& data, const char *filename)
{
	GDALAllRegister();

	GDALDataset* poDataset;
	poDataset = (GDALDataset*) GDALOpen(filename, GA_Update);
	if ( !poDataset ) return 3;

	GDALRasterBand* poBand;        
	poBand = poDataset->GetRasterBand(1);
	int nXSize = poBand->GetXSize();
	int nYSize = poBand->GetYSize();

	double* pData;
	pData = (double*)CPLMalloc(sizeof(double)*nXSize*nYSize);
	std::copy(data.begin(), data.end(), pData);

	int ret = poBand->RasterIO( GF_Write,
				0, 0, nXSize, nYSize,
				pData, nXSize, nYSize,
				GDT_Float64, 0, 0);

	CPLFree(pData);
	GDALClose(poDataset);

	return ret;
}

int copyRaster(const char *oldfilename, const char *newfilename)
{
	GDALAllRegister();

	// delete existed newfile
	GDALDriver::QuietDelete(newfilename);

	// copy and create newfile, only support GTiff
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	
	return poDriver->CopyFiles(newfilename, oldfilename);
}

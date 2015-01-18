#include "raster.h"
#include "gdal_priv.h"
#include "cpl_conv.h"


int readRaster(Map<int>& datamap, const char *filename)
{
    GDALAllRegister();

    GDALDataset* poDataset;
    poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
    if (!poDataset) return 3;

    GDALRasterBand* poBand;
    poBand = poDataset->GetRasterBand(1);
    int nXSize = poBand->GetXSize();
    int nYSize = poBand->GetYSize();
    double nodata = poBand->GetNoDataValue();
    datamap.xsize = nXSize;
    datamap.ysize = nYSize;
    datamap.nodata = nodata;

    int* pData;
    pData = (int*)CPLMalloc(sizeof(int)*nXSize*nYSize);
    int ret = poBand->RasterIO(GF_Read,
                0, 0, nXSize, nYSize,
                pData, nXSize, nYSize,
                GDT_Int32, 0, 0);

    datamap.assign(pData, pData + nXSize * nYSize);

    CPLFree(pData);
    GDALClose(poDataset);

    return ret;
}

int readRaster(Map<double>& datamap, const char *filename)
{
    GDALAllRegister();

    GDALDataset* poDataset;
    poDataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
    if (!poDataset) return 3;

    GDALRasterBand* poBand;
    poBand = poDataset->GetRasterBand(1);
    int nXSize = poBand->GetXSize();
    int nYSize = poBand->GetYSize();
    double nodata = poBand->GetNoDataValue();
    datamap.xsize = nXSize;
    datamap.ysize = nYSize;
    datamap.nodata = nodata;

    double* pData;
    pData = (double*)CPLMalloc(sizeof(double)*nXSize*nYSize);
    int ret = poBand->RasterIO(GF_Read,
                0, 0, nXSize, nYSize,
                pData, nXSize, nYSize,
                GDT_Float64, 0, 0);

    datamap.assign(pData, pData + nXSize * nYSize);

    CPLFree(pData);
    GDALClose(poDataset);

    return ret;
}

int writeRaster(const Map<int>& datamap, const char *filename)
{
    GDALAllRegister();

    GDALDataset* poDataset;
    poDataset = (GDALDataset*) GDALOpen(filename, GA_Update);
    if (!poDataset) return 3;

    GDALRasterBand* poBand;
    poBand = poDataset->GetRasterBand(1);
    int nXSize = poBand->GetXSize();
    int nYSize = poBand->GetYSize();
    poBand->SetNoDataValue(datamap.nodata);

    int* pData;
    pData = (int*)CPLMalloc(sizeof(int)*nXSize*nYSize);
    std::copy(datamap.begin(), datamap.end(), pData);

    int ret = poBand->RasterIO(GF_Write,
                0, 0, nXSize, nYSize,
                pData, nXSize, nYSize,
                GDT_Int32, 0, 0);

    CPLFree(pData);
    GDALClose(poDataset);

    return ret;
}

int writeRaster(const Map<double>& datamap, const char *filename)
{
    GDALAllRegister();

    GDALDataset* poDataset;
    poDataset = (GDALDataset*) GDALOpen(filename, GA_Update);
    if (!poDataset) return 3;

    GDALRasterBand* poBand;
    poBand = poDataset->GetRasterBand(1);
    int nXSize = poBand->GetXSize();
    int nYSize = poBand->GetYSize();
    poBand->SetNoDataValue(datamap.nodata);

    double* pData;
    pData = (double*)CPLMalloc(sizeof(double)*nXSize*nYSize);
    std::copy(datamap.begin(), datamap.end(), pData);

    int ret = poBand->RasterIO(GF_Write,
                0, 0, nXSize, nYSize,
                pData, nXSize, nYSize,
                GDT_Float64, 0, 0);

    CPLFree(pData);
    GDALClose(poDataset);

    return ret;
}

int writeRaster(const Map<int>& datamap, const char *filename,
    const char *ref_file)
{
    if (copyRaster(ref_file, filename)) return 3;

    if (writeRaster(datamap, filename)) return 3;

    return 0;
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

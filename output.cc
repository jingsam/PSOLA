#include <string>
#include "option.h"
#include "raster.h"
#include "output.h"

void init_output()
{
    std::string cmd = "mkdir -p " + g_output;
    system(cmd.c_str());
}

tinyxml2::XMLDocument* createLogDocument()
{
    /*      <?xml version="1.0" encoding="UTF-8"?>
     *      <result>
     *          <map>
     *              <row>546</row>
     *              <col>546</col>
     *          </map>
     *      </result>
     */
    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
    tinyxml2::XMLDeclaration* declr = doc->NewDeclaration();
    doc->InsertEndChild(declr);
    tinyxml2::XMLElement* root = doc->NewElement("result");
    doc->InsertEndChild(root);
    tinyxml2::XMLElement* map = doc->NewElement("map");
    root->InsertEndChild(map);
    tinyxml2::XMLElement* row = doc->NewElement("row");
    map->InsertEndChild(row);
    tinyxml2::XMLElement* col = doc->NewElement("col");
    map->InsertEndChild(col);

    char buffer[50];
    std::sprintf(buffer, "%d", g_ysize);
    row->InsertEndChild(doc->NewText(buffer));
    std::sprintf(buffer, "%d", g_xsize);
    col->InsertEndChild(doc->NewText(buffer));

    return doc;
}

void logStatus(tinyxml2::XMLDocument* doc, 
    int generation, double fitness, const char *filename)
{
    /*      <iter>
     *          <gen>1</gen>
     *          <fitness>0.82</fitness>
     *          <file>1.tif</file>
     *      </iter>
     *
     */
    tinyxml2::XMLElement* root = doc->RootElement();
    tinyxml2::XMLElement* iter = doc->NewElement("iter");
    root->InsertEndChild(iter);
    tinyxml2::XMLElement* gen = doc->NewElement("gen");
    iter->InsertEndChild(gen);
    tinyxml2::XMLElement* fit = doc->NewElement("fitness");
    iter->InsertEndChild(fit);
    tinyxml2::XMLElement* file = doc->NewElement("file");
    iter->InsertEndChild(file);

    char buffer[50];
    std::sprintf(buffer, "%d", generation);
    gen->InsertEndChild(doc->NewText(buffer));
    std::sprintf(buffer, "%f", fitness);
    fit->InsertEndChild(doc->NewText(buffer));
    if (filename) file->InsertEndChild(doc->NewText(filename));   
}

int outputImage(const std::vector<int>& data, const char *filename)
{
    if (copyRaster(g_region.c_str(), filename)) {
        std::printf("Failed copy %s to %s\n", g_region.c_str(), filename);
    }

    if (writeRaster(data, filename)) {
        std::printf("Failed write result to %s\n", filename);
        return 3;
    }

    return 0;
}
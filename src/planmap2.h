#ifndef PLANMAP_H
#define PLANMAP_H

#include <vector>
#include <algorithm>    // find()
#include <map>
#include <string>
#include "datamap.h"    // Map<T>


class Cell;
class Patch;
class Class;
class Landscape;

class Cell
{
public:
    Cell() {}

    ~Cell() {}


    int x, y;
    int value;
    int type;  // 0-invariable, 1-variable
    std::vector<double> transP;
    Patch* patch;
};


class Patch : public std::vector<Cell*>
{
public:
    Patch() {}

    ~Patch() {}


    int value;
    Class* cls;
};


class Class : public std::vector<Patch*>
{
public:
    Class() {}

    ~Class() { }

    int value;
    Landscape* landscape;
};


class Landscape : public Map<Cell*>
{
public:
    Landscape() {}

    ~Landscape() {
        for (int i = 0; i < patches->size(); ++i) {
            delete patches->at(i);
        }

        for (int j = 0; j < classes->size(); ++j) {
            delete classes->at(j);
        }

        for (int k = 0; k < this->size(); ++k) {
            delete this->at(k);
        }
    }


    std::vector<Patch*> patches;
    std::vector<Class*> classes;
    std::map<std::string, double> stats;
};

#endif

#ifndef PLANMAP_H
#define PLANMAP_H

#include <vector>
#include <map>
#include <string>
#include "datamap.h"    // Map<T>


class Cell;
class PlanMap;
void statistics(PlanMap* plan_map);

class Cell
{
public:
    Cell() {}

    Cell(int x, int y, int value) {
        this->x = x;
        this->y = y;
        this->value = value;
    }

    ~Cell() {}

    Cell* clone() {
        Cell* cell = new Cell(this->x, this->y, this->value);
        cell->type = this->type;
        cell->transP = this->transP;

        return cell;
    }


    int x, y;
    int value;
    int type;  // 0-invariable, 1-variable
    std::vector<double> transP;
    PlanMap* map;

};

class PlanMap : public Map<Cell*>
{
public:
    PlanMap() {}

    PlanMap(int xsize, int ysize, int value, double nodata) {
        this->xsize = xsize;
        this->ysize = ysize;
        this->nodata = nodata;
        for (int y=0; y < ysize; ++y) {
            for (int x=0; x < xsize; ++x) {
                Cell* cell = new Cell(x, y, value);
                cell->map = this;
                this->push_back(cell);
            }
        }
    }

    PlanMap(Map<int>& datamap) {
        this->xsize = datamap.xsize;
        this->ysize = datamap.ysize;
        this->nodata = datamap.nodata;
        for (int y=0; y < ysize; ++y) {
            for (int x=0; x < xsize; ++x) {
                Cell* cell = new Cell(x, y, datamap.atxy(x, y));
                cell->map = this;
                this->push_back(cell);
            }
        }
    }

    ~PlanMap() {
        for (int i = 0; i < this->size(); ++i) {
            delete this->at(i);
        }
    }

    bool betterThan(PlanMap* other) {
        return this->stats["fitness"] > other->stats["fitness"];
    }

    void updateStats() {
        statistics(this);
    }

    PlanMap* clone() {
        PlanMap* map = new PlanMap();
        map->xsize = this->xsize;
        map->ysize = this->ysize;
        map->nodata = this->nodata;
        map->stats = this->stats;

        for (int i = 0; i < this->size(); ++i) {
            Cell* cell = this->at(i)->clone();
            cell->map = map;
            map->push_back(cell);
        }

        return map;
    }

    Map<int> getDataMap() {
        Map<int> datamap(xsize, ysize, 0, this->nodata);
        for (int i = 0; i < this->size(); ++i) {
            datamap.at(i) = this->at(i)->value;
        }

        return datamap;
    }

    void setDataMap(Map<int>& datamap) {
        for (int i = 0; i < this->size(); ++i) {
            this->at(i)->value = datamap.at(i);
        }
        this->updateStats();
    }

    void setDataMap(PlanMap* datamap) {
        for (int i = 0; i < this->size(); ++i) {
            this->at(i)->value = datamap->at(i)->value;
        }
        this->stats = datamap->stats;
    }


    std::map<std::string, double> stats;
};

#endif

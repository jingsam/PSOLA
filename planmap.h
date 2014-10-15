#ifndef PLANMAP_H
#define PLANMAP_H

#include <vector>
#include <map>
#include "datamap.h"    // class Map<T>


double calc_fitness(Map<int>& scheme);

class Cell;
class PlanMap;

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
        Cell* cell = new Cell();
        cell->x = this->x;
        cell->y = this->y;
        cell->value = this->value;
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

    PlanMap(int xsize, int ysize, int defaultValue) {
        this->xsize = xsize;
        this->ysize = ysize;
        for (int y=0; y < ysize; ++y) {
            for (int x=0; x < xsize; ++x) {
                Cell* cell = new Cell(x, y, defaultValue);
                cell->map = this;
                this->push_back(cell);
            }
        }
        this->fitness = 0.0;
    }

    ~PlanMap() {
        for (int i = 0; i < this->size(); ++i) {
            delete this->at(i);
        }
    }

    bool betterThan(PlanMap* other) {
        return this->fitness > other->fitness;
    }

    void updateFitness() {
        Map<int> map = this->getDataMap();
        this->fitness = calc_fitness( map );
    }

    Map<int> getDataMap() {
        Map<int> map( xsize, ysize, 0 );
        for (int i = 0; i < this->size(); ++i) {
            map.at(i) = this->at(i)->value;
        }

        return map;
    }

    PlanMap* clone() {
        PlanMap* map = new PlanMap();
        map->xsize = this->xsize;
        map->ysize = this->ysize;
        map->fitness = this->fitness;
        map->counts = this->counts;

        for (int i = 0; i < this->size(); ++i) {
            Cell* cell = this->at(i)->clone();
            cell->map = map;
            map->push_back(cell);
        }

        return map;
    }

    void assignValue(PlanMap* other) {
        this->fitness = other->fitness;
        this->counts = other->counts;
        for (int i = 0; i < this->size(); ++i) {
            this->at(i)->value = other->at(i)->value;
        }
    }

    void assignValue(Map<int>& other) {
        this->fitness = calc_fitness(other);
        for (int i = 0; i < this->size(); ++i) {
            this->at(i)->value = other.at(i);
        }
    }

    double fitness;
    std::map<int, int> counts;
};

#endif

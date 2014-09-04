#ifndef PLANMAP_H
#define PLANMAP_H

#include <vector>
#include "datamap.h"	// class Map<T>


double calc_fitness(Map<int>& scheme);

class Cell;
class PlanMap;

enum CellType {
	kNormalCell = 0,
	kBackgroundCell,
	kExcludedCell,
	kDeterminedCell,
	kMaxCellType
};

class Cell
{
public:
	Cell() {}

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
	CellType type;
	std::vector<double> transP;
	PlanMap* map;

};

class PlanMap : public Map<Cell*>
{
public:
	PlanMap() {}

	~PlanMap() {
		for (int i = 0; i < this->size(); ++i) {
		    delete this->at(i);
		}
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

		for (int i = 0; i < this->size(); ++i) {
			Cell* cell = this->at(i)->clone();
			cell->map = map;			
			map->push_back(cell);
		}

		return map;
	}

	void assignValue(PlanMap* map) {
		this->fitness = map->fitness;
		for (int i = 0; i < this->size(); ++i) {
			this->at(i)->value = map->at(i)->value;
		}
	}
	
	double fitness;
};

#endif

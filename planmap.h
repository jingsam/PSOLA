#ifndef PLANMAP_H
#define PLANMAP_H

#include <vector>
#include "datamap.h"	// class Map<T>


double calc_fitness(Map<int>& scheme);

class Cell;
class PlanMap;
class Stats;

enum CellType {
	kNormalCell = 0,
	kBackgroundCell,
	kExcludedCell,
	kDeterminedCell,
	kMaxCellType
};

class Stats
{
public:
	Stats() {}

	~Stats() {}

	void reset() {
		fitness = 0.0;
		counts.assign(counts.size(), 0);
	}

	void equalTo(Stats* stats) {
		this->fitness = stats->fitness;
		this->counts = stats->counts;
	}

	bool betterThan(Stats* stats) {
		return this->fitness > stats->fitness;
	}
	
	double fitness;
	std::vector<int> counts;
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
	
	void equalTo(Cell* cell) {
		this->value = cell->value;
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
		delete stats;
		for (int i = 0; i < this->size(); ++i) {
		    delete this->at(i);
		}
	}

	void updateFitness() {
		Map<int> map = this->getDataMap();
		this->stats->fitness = calc_fitness( map );
	}

	Map<int> getDataMap() {
		Map<int> map( xsize * ysize, 0 );
		map.xsize = this->xsize;
		map.ysize = this->ysize;

		for (int i = 0; i < this->size(); ++i) {
		    map.at(i) = this->at(i)->value;
		}

		return map;
	}
	
	// std::vector<Cell*> getPatch(int x, int y) {
	// 	std::vector<Cell*> result;
	// 	std::vector<Cell*> neighbors = this->neighbors4( x, y, 1 );
	// 	for (int i = 0; i < neighbors.size(); ++i) {
	// 		if ()
	// 	}
	// }

	PlanMap* clone() {
		PlanMap* map = new PlanMap();
		map->xsize = this->xsize;
		map->ysize = this->ysize;
		map->stats = new Stats();
		map->stats->equalTo( this->stats );

    	for (int i = 0; i < this->size(); ++i) {
    	    Cell* cell = this->at(i)->clone();
    	    cell->map = map;			
    	    map->push_back(cell);
    	}

    	return map;
	}

	void equalTo(PlanMap* map) {
		this->stats->equalTo( map->stats );
    	for (int i = 0; i < this->size(); ++i) {
        	this->at(i)->equalTo( map->at(i) );
    	}
	}
	
	bool betterThan(PlanMap* map) {
		return this->stats->betterThan( map->stats );
	}


	Stats* stats;
};

#endif

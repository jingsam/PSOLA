#include "rule.h"
#include "option.h"

void normalize(std::vector<double>& p);
int roulette_wheel(std::vector<double>& p, Random* rnd);


void rule_grain_for_green(Cell* cell)
{
    double slope = g_slope_map.atxy(cell->x, cell->y);
    if (slope >= 25.0) {
        cell->value = 3;
        cell->type = 0;
    }
}

void rule_soil_conservation(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    std::vector<int> neighbors = g_land_use_map.neighbors(cell->x, cell->y, 2);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i) == 8) {
            cell->type = 0;
        }
    }
}

void rule_quantity_constraint(Cell* cell) 
{
    
}

void rule_farming_radius(Cell* cell) 
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, 40);
    for (int i=0; i < neighbors.size(); ++i) {
    	if (neighbors.at(i)->value == 6) return;
    }

    cell->transP.at(0) = 0.0;
}

void rule_transportation(Cell* cell) 
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, 20);
    for (int i=0; i < neighbors.size(); ++i) {
    	if (neighbors.at(i)->value == 7) return;
    }
}

int transition(Cell* cell)
{
    return roulette_wheel(cell->transP, RND);
}

void int_cell(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    cell->value = land_use;
    mycell->type = 1;
    cell->transP.assign(g_max, 1.0 / g_max);
    
    if (land_use == g_nodata) {
	    mycell->type = 0;
	    return;
    }
	
    switch (land_use) {
        case 1:
            rule_grain_for_green(cell);
            break;
        case 4:
            rule_soil_conservation(cell);
            break;
        case 5:  case 7:  case 8:  case 9: 
            cell->type = 0;
            break;
     }
}

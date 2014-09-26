#include "rule.h"
#include "option.h"

void normalize(std::vector<double>& p);
int rouletteWheel(std::vector<double>& p, Random* rnd);
std::vector<int> merge_rule(std::vector<int>& rule1, std::vector<int>& rule2);
std::vector<double> take_rule(std::vector<int>& rule, std::vector<double>& p);


void grain_for_green(Cell* cell);
std::vector<int> quantity_constraint(Cell* cell);


int transition(Cell* cell)
{
    // rules
    std::vector<int> rule1 = quantity_constraint( cell );

    // transition
    std::vector<double> p = take_rule( rule1, mycell->transP );
    // mycell->value = roulette_wheel(p, &RND);

    mycell->value = roulette_wheel(mycell->transP, RND);

    return mycell->value;
}


std::vector<int> merge_rule(std::vector<int>& rule1, std::vector<int>& rule2)
{
    std::vector<int> result = rule1;
    for (int i = 0; i < result.size(); ++i) {
        if (rule2.at(i) == 0) result.at(i) = 0;
    }

    return result;
}

std::vector<double> take_rule(std::vector<int>& rule, std::vector<double>& p) 
{
    std::vector<double> result = p;
    for (int i = 0; i < p.size(); ++i) {
        if (rule.at(i) == 0) result.at(i) = 0;
    }
    normalize(result);

    return result;
}

void int_cell(Cell* cell)
{
    int land_use = g_land_use_map.at(cell->x, cell->y);
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

void rule_grain_for_green(Cell* cell)
{
    double slope = g_slope_map.at(cell->x, cell->y);
    if (slope >= 25.0) {
        cell->value = 3;
        cell->type = 0;
    }
}

void rule_soil_conservation(Cell* cell)
{
    int land_use = g_land_use_map.at(cell->x, cell->y);
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



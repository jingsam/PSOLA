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
    normalize_p(result);

    return result;
}

void int_cell(Cell* cell)
{
    int land_use = g_land_use_map.at(x, y);
    cell->value = land_use;
    mycell->type = 1;
    cell->transP.assign(g_max, 1.0 / g_max);
    
    if (land_use == g_nodata) {
	    mycell->type = 0;
	    return;
    }
	
    switch (land_use) {
        case 1:
            grain_for_green(cell);
            break;
        case 5:  case 7:  case 8:  case 9: 
            cell->type = 0;
            break;
     }
}

void grain_for_green(Cell* cell)
{
    double slope = g_slope_map.at(cell->x, cell->y);
    if (slope >= 25.0) {
        mycell->value = 3;
        mycell->type = 0;
    }
}

void quantity_constraint(Cell* cell) {
    std::vector<int> counts = cell->map->stats->counts;
    std::vector<int> flags( counts.size(), 0 );
    for (int i = 0; i < counts.size(); ++i) {
        if (counts.at(i) < g_land_use_struct.at(i)) {
            flags.at(i) = 1;
        }
    }

    return flags;
}

void farming_radius(Cell* cell) {
    std::vector<int> flags( cell->transP.size(), 1 );

    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, 40);
    for (int i=0; i < neighbors.size(); ++i) {
    	if (neighbors.at(i)->value == 6) return;
    }

    cell->transP.at(0) = 0.0;
}

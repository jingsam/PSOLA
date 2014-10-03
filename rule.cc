#include "rule.h"
#include "option.h"

void normalize(std::vector<double>& p);
int roulette_wheel(std::vector<double>& p, Random* rnd);

int transition(Cell* cell);
void int_cell(Cell* cell);
void rule_grain_for_green(Cell* cell);
void rule_soil_conservation(Cell* cell);
void rule_quantity_arableland(Cell* cell);
void rule_farming_radius(Cell* cell);
void rule_transportation(Cell* cell);
bool is_edge_cell(Cell* cell);


int transition(Cell* cell)
{
    if (cell->type == 0) return cell->value;

    double p = g_RND->nextDouble();
    if (is_edge_cell(cell)) {
        if (p < g_edge) return cell->value;
    } else {
        if (p < g_core) return cell->value;
    }

    int new_value = roulette_wheel(cell->transP, g_RND);
    switch (new_value) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
    }

    return new_value;
}

void int_cell(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    cell->value = land_use;
    cell->type = 1;
    cell->transP.assign(g_max, 1.0 / g_max);

    if (land_use == g_nodata) {
        cell->type = 0;
        return;
    }

    switch (land_use) {
        case 1:
            rule_grain_for_green(cell);
            break;
        case 4:
            rule_soil_conservation(cell);
            break;
        case 5:
        case 7:
        case 8:
        case 9:
            cell->type = 0;
            break;
     }
}

bool is_edge_cell(Cell* cell)
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, g_edge_depth);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            return true;
        }
    }

    return false;
}

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

void rule_quantity_arableland(Cell* cell)
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



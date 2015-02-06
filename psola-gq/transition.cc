#include "transition.h"
#include "parameter.h" // g_XX_suit_map, g_slope_map, g_road_map


int transition(Cell* cell);
bool rule_max_arable(Cell* cell);
bool rule_max_construction(Cell* cell);
bool rule_conserve_arable(Cell* cell);
bool rule_neighbors_has(Cell* cell, int radius, int value);
bool rule_farming_radius(Cell* cell, int radius);
bool rule_road_access(Cell* cell, double max_distance);
bool rule_suitability(Cell* cell, int value, double min_suit);

int transition(Cell* cell)
{
    int x = cell->x;
    int y = cell->y;

    if (cell->type == 0) return cell->value;

    // undeveloped area
    int land_use = g_land_use_map.atxy(x, y);
    if (land_use == 4 || land_use == 5 || land_use > 6)
    {
        cell->type = 0;
        return land_use;
    }

    // grain for green
    if (land_use == 1)
    {
        double slope = g_slope_map.atxy(x, y);
        if (slope >= 25.0)
        {
            cell->type = 0;
            return 3;
        }
    }

    // core-edge operator
    /*int depth_of_edge = stoi(g_option["depth-of-edge"]);
    double g_core = stod(g_option["core"]);
    double g_edge = stod(g_option["edge"]);
    bool is_core_cell = true;
    std::vector<Cell*> neighbors = cell->map->neighbors(x, y, depth_of_edge);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            is_core_cell = false;
            break;
        }
    }

    double p = g_rnd->nextDouble();
    if ((is_core_cell && p > g_core) ||
        (!is_core_cell && p > g_edge)) {
        return cell->value;
    }*/

    // roulette_wheel
    bool confirmed = false;
    int new_value = g_rnd->rouletteWheel(cell->transP);
    if (new_value < 7 && new_value != 4) confirmed = true;
    /*switch (new_value) {
        case 1:
            confirmed =
                rule_max_arable(cell) &&
                rule_suitability(cell, 1, 0.6) &&
                rule_neighbors_has(cell, 40, 6);
            break;
        case 2:
            confirmed =
                rule_conserve_arable(cell) &&
                rule_suitability(cell, 2, 0.3) &&
                rule_road_access(cell, 500.0);
            break;
        case 3:
            confirmed =
                rule_conserve_arable(cell) &&
                rule_suitability(cell, 3, 0.3);
            break;
        case 5:
            confirmed =
                rule_max_construction(cell) &&
                rule_neighbors_has(cell, 1, 5);
            break;
        case 6:
            confirmed =
                rule_max_construction(cell) &&
                rule_neighbors_has(cell, 1, 6);
            break;
    }*/

    return confirmed ? new_value : cell->value;
}

bool rule_max_arable(Cell* cell)
{
    int max = stoi(g_option["arable"]);
    int count = (int)cell->map->stats["1"];
    return count < max;
}

bool rule_conserve_arable(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    double suit = g_arable_suit_map.atxy(cell->x, cell->y);

    return !(land_use == 1 && suit > 0.3);
}

bool rule_max_construction(Cell* cell)
{
    int max = stoi(g_option["construction"]);
    int urban = (int)cell->map->stats["5"];
    int rural = (int)cell->map->stats["6"];
    int mine = (int)cell->map->stats["7"];
    int road = (int)cell->map->stats["8"];
    return (urban + rural + mine + road) < max;
}

bool rule_neighbors_has(Cell* cell, int radius, int value)
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == value) return true;
    }

    return false;
}

bool rule_road_access(Cell* cell, double max_distance)
{
    double distance = g_road_map.atxy(cell->x, cell->y);

    return distance <= max_distance;
}

bool rule_suitability(Cell* cell, int value, double min_suit)
{
    double suit = 0.0;
    switch (value)
    {
        case 1:
            suit = g_arable_suit_map.atxy(cell->x, cell->y);
            break;
        case 2:
            suit = g_orchard_suit_map.atxy(cell->x, cell->y);
            break;
        case 3:
            suit = g_forest_suit_map.atxy(cell->x, cell->y);
            break;
        case 5:
        case 6:
            suit = g_construction_suit_map.atxy(cell->x, cell->y);
            break;
    }

    return suit >= min_suit;
}

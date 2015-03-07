#include "../../src/psola.h"
#include "parameter.h"


bool rule_max_arable(Cell* cell);
bool rule_conserve_arable(Cell* cell);
bool rule_farming_radius(Cell* cell, int radius);
bool rule_road_access(Cell* cell, double max_distance);
bool rule_suitability(Cell* cell, int value, double min_suit);
bool rule_max_slope(Cell* cell, double max);
bool rule_village_size(Cell* cell, int size);

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

    //neighbors_operator(cell, 1);

    // core-edge operator
    // double p = g_rnd->nextDouble();
    // if (!core_edge_operator(cell, p)) return cell->value;

    // roulette_wheel
    bool confirmed = false;
    int new_value = roulette_wheel(cell, g_rnd);
    switch (new_value) {
        case 1:
            confirmed = true;
            break;
        case 2:
            confirmed = true;
            break;
        case 3:
            confirmed = true;
            break;
        case 5:
            confirmed = true &&
                neighbors_has(cell, 5, 1);
            break;
        case 6:
            confirmed = true;
            break;
    }

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

bool rule_max_slope(Cell* cell, double max)
{
    int slope = g_slope_map.atxy(cell->x, cell->y);

    return slope <= max;
}

bool rule_village_size(Cell* cell, int size)
{
    std::vector<Cell*> patch = cell->map->getPatch(cell->x, cell->y, true);

    return patch.size() >= size;
}

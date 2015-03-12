#include "../../src/psola.h"
#include "parameter.h"


bool suit_for_use(Cell* cell, int value);
bool max_slope(Cell* cell, double threshold);
bool min_road_access(Cell* cell, double threshold);
bool min_patch_size(Cell* cell, int value, int threshold);
bool min_suit(Cell* cell, int value, double threshold);
bool in_urban(Cell* cell);

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
    // double p = g_rnd->nextDouble();
    // if (!core_edge_operator(cell, p)) return cell->value;

    // check current use

    if (in_urban(cell) && neighbors_has(cell, 5, 1))
    {
        return 5;
    }


    // roulette_wheel
    int new_value = roulette_wheel(cell, g_rnd);
    bool suit = suit_for_use(cell, new_value);
    return suit ? new_value : cell->value;
}


bool suit_for_use(Cell* cell, int value) {
    bool suit = false;
    switch (value) {
        case 1:
            suit = true ||
                min_suit(cell, 1, 0.6) &&
                max_slope(cell, 25.0) &&
                neighbors_has(cell, 6, 40);
            break;
        case 2:
            suit = true ||
                min_suit(cell, 2, 0.6) &&
                min_road_access(cell, 1000.0);
            break;
        case 3:
            suit = true ||
                min_suit(cell, 3, 0.6);
            break;
        case 5:
            suit = true &&
                neighbors_has(cell, 5, 1) &&
                in_urban(cell);
            break;
        case 6:
            suit = true ||
                min_suit(cell, 6, 0.6) &&
                neighbors_has(cell, 6, 1);
            break;
    }

    return suit;
}

bool max_slope(Cell* cell, double threshold)
{
    int slope = g_slope_map.atxy(cell->x, cell->y);

    return slope <= threshold;
}

bool min_road_access(Cell* cell, double threshold)
{
    double distance = g_road_map.atxy(cell->x, cell->y);

    return distance <= threshold;
}

bool min_patch_size(Cell* cell, int value, int threshold)
{
    int tmp = cell->value;
    cell->value = value;
    std::vector<Cell*> patch = cell->map->getPatch(cell->x, cell->y);
    cell->value = tmp;

    return patch.size() >= threshold;
}

bool min_suit(Cell* cell, int value, double threshold)
{
    int x = cell->x;
    int y = cell->y;

    double suit = 0.0;
    switch (value) {
        case 1: suit = g_arable_suit_map.atxy(x, y);       break;
        case 2: suit = g_orchard_suit_map.atxy(x, y);      break;
        case 3: suit = g_forest_suit_map.atxy(x, y);       break;
        case 5: suit = g_construction_suit_map.atxy(x, y); break;
        case 6: suit = g_construction_suit_map.atxy(x, y); break;
    }

    return suit >= threshold;
}

bool in_urban(Cell* cell)
{
    int value = g_urban_map.atxy(cell->x, cell->y);

    return value == 1;
}

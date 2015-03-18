#include "../../src/psola.h"
#include "parameter.h"


bool suit_for_use(Cell* cell, int value);
bool conv_for_use(Cell* cell, int value);
bool max_slope(Cell* cell, double threshold);
bool min_road_access(Cell* cell, double threshold);
bool min_patch_size(Cell* cell, int value, int threshold);
bool min_suit(Cell* cell, int value, double threshold);
bool in_urban(Cell* cell);
void suit_effects(Cell* cell);

int transition(Cell* cell)
{
    if (cell->type == 0) return cell->value;

    int x = cell->x;
    int y = cell->y;
    int value = cell->value;
    std::vector<double> transP = cell->transP;
    int land_use = g_land_use_map.atxy(x, y);

    // undeveloped area
    if (land_use == 3 || land_use == 4 || land_use >= 6) {
        cell->type = 0;
        return land_use;
    }

    // grain for green
    if (land_use == 0) {
        if (g_slope_map.atxy(x, y) >= 25.0) {
            cell->type = 0;
            return 2;
        }
    }

    neighbor_effects(cell, 1);
    suit_effects(cell);

    int new_value = g_rnd->nextInt(cell->transP);
    cell->transP = transP;

    return suit_for_use(cell, new_value) ? new_value : value;
}


bool suit_for_use(Cell* cell, int value) {
    if (value == 3 || value >= 6) return false;
    if (!conv_for_use(cell, value)) return false;

    bool suit = false;
    switch (value) {
        case 0:
            suit = true &&
                neighbors_has(cell, 0, 1);
            break;
        case 1:
            suit = true &&
                neighbors_has(cell, 1, 1);
            break;
        case 2:
            suit = true &&
                neighbors_has(cell, 2, 1);
            break;
        case 4:
            suit = true &&
                neighbors_has(cell, 4, 1) &&
                in_urban(cell);
                if (suit) cell->type = 0;
            break;
        case 5:
            suit = true &&
                neighbors_has(cell, 5, 1);
            break;
    }

    return suit;
}

bool conv_for_use(Cell* cell, int value)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    switch (land_use) {
        case 0:
            return value == 0 || value == 4 || value == 5;
        case 1:
        case 2:
            return value == 0 || value == 1 || value == 2 ||
                   value == 4 || value == 5;
        case 4:
            return value == 4;
        case 5:
            return value ==0 || value == 4 || value == 5;
    }

    return true;
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
        case 0: suit = g_arable_suit_map.atxy(x, y);       break;
        case 1: suit = g_orchard_suit_map.atxy(x, y);      break;
        case 2: suit = g_forest_suit_map.atxy(x, y);       break;
        case 4: suit = g_construction_suit_map.atxy(x, y); break;
        case 5: suit = g_construction_suit_map.atxy(x, y); break;
    }

    return suit >= threshold;
}

bool in_urban(Cell* cell)
{
    int value = g_urban_map.atxy(cell->x, cell->y);

    return value == 1;
}

void suit_effects(Cell* cell)
{
    int x = cell->x;
    int y = cell->y;

    double suit0 = g_arable_suit_map.atxy(x, y);
    double suit1 = g_orchard_suit_map.atxy(x, y);
    double suit2 = g_forest_suit_map.atxy(x, y);
    double suit4 = g_construction_suit_map.atxy(x, y);
    double suit5 = g_construction_suit_map.atxy(x, y);

    cell->transP.at(0) += suit0;
    cell->transP.at(1) += suit1;
    cell->transP.at(2) += suit2;
    cell->transP.at(4) += suit4;
    cell->transP.at(5) += suit5;

    normalize(cell->transP);
}

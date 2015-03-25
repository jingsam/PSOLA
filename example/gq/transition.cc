#include "../../src/psola.h"
#include "parameter.h"


bool suit_for_use(Cell* cell, int value);
bool conv_for_use(Cell* cell, int value);
double suit(Cell* cell, int value);
void suit_effects(Cell* cell);
int neighbors_count_big_village(Cell* cell, int radius);

int transition(Cell* cell)
{
    if (cell->type == 0) return cell->value;

    int x = cell->x;
    int y = cell->y;
    int value = cell->value;
    std::vector<double> transP = cell->transP;

    // grain for green
    int land_use = g_land_use_map.atxy(x, y);
    if (land_use == 0) {
        if (g_slope_map.atxy(x, y) >= 25.0) {
            cell->type = 0;
            return 2;
        }
    }

    neighbor_effects(cell, 1);
    suit_effects(cell);

    int new_value = g_rnd->nextInt(cell->transP);
    // int new_value = core_edge_operator(cell, g_rnd);
    cell->transP = transP;

    bool suit = suit_for_use(cell, new_value);
    if (new_value == 4 && suit) cell->type = 0;

    return suit ? new_value : value;
}


bool suit_for_use(Cell* cell, int value) {
    int x = cell->x;
    int y = cell->y;

    if (!conv_for_use(cell, value)) return false;

    switch (value) {
        case 0:
            return suit(cell, value) >= 0.3 &&
                g_slope_map.atxy(x, y) < 25.0 &&
                neighbors_count(cell, value, 1) >= 3 &&
                neighbors_count(cell, 5, 40) >= 1;
        case 1:
            return suit(cell, value) >= 0.6 &&
                g_road_map.atxy(x, y) <= 1000.0 &&
                neighbors_count(cell, value, 1) >= 4;
        case 2:
            return suit(cell, value) >= 0.6 &&
                neighbors_count(cell, value, 1) >= 4;
        case 4:
            return g_urban_map.atxy(x, y) == 1 &&
                neighbors_count(cell, value, 1) >= 1;
        case 5:
            return neighbors_count_big_village(cell, 1) >= 6;
    }


    return false;
}

bool conv_for_use(Cell* cell, int value)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);
    switch (land_use) {
        case 0: return value == 0 || value == 4 || value == 5;
        case 1:
        case 2: return value == 0 || value == 1 || value == 2 || value == 4 || value == 5;
        case 3: return value == 3;
        case 4: return value == 4;
        case 5: return value == 0 || value == 4 || value == 5;
        case 6:
        case 7:
        case 8:
        case 9: return value == land_use;
    }

    return false;
}

double suit(Cell* cell, int value)
{
    int x = cell->x;
    int y = cell->y;

    switch (value) {
        case 0: return g_arable_suit_map.atxy(x, y);
        case 1: return g_orchard_suit_map.atxy(x, y);
        case 2: return g_forest_suit_map.atxy(x, y);
        case 4:
        case 5: return g_construction_suit_map.atxy(x, y);
        default: return 0.0;
    }
}

void suit_effects(Cell* cell)
{
    for (int i = 0; i < cell->transP.size(); ++i) {
        cell->transP.at(i) += suit(cell, cell->value);
    }

    normalize(cell->transP);
}

int neighbors_count_big_village(Cell* cell, int radius)
{
    int count = 0;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == 5 &&
            neighbors.at(i)->type == 0) count++;
    }

    return count;
}

#include "transition.h"
#include <string>      // stoi(), stod()
#include "tool.h"      // normalize(), roulette_wheel()
#include "option.h"    // g_option
#include "parameter.h" // g_land_use_map, g_XX_suit_map, g_slope_map, g_road_map


int transition(Cell* cell);
bool rule_cell_type(Cell* cell);
bool rule_edge_cell(Cell* cell);
bool rule_quantity(Cell* cell, int value, int max);
bool rule_neighbors_has(Cell* cell, int radius, int value);
bool rule_soil_condition(Cell* cell);
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
        cell->type == 0;
        return land_use;
    }

    // grain for green
    if (land_use == 1)
    {
        double slope = g_slope_map.atxy(x, y);
        if (slope >= 25.0)
        {
            cell->type == 0;
            return 3;
        }
    }

    // core-edge operator
    int depth_of_edge = std::stoi(g_option["depth-of-edge"]);
    double g_core = std::stod(g_option["core"]);
    double g_edge = std::stod(g_option["edge"]);
    bool is_core_cell = true;
    std::vector<Cell*> neighbors = cell->map->neighbors(x, y, depth_of_edge);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            is_edge_cell = false;
            break;
        }
    }

    double p = g_RND->nextDouble();
    if (is_core_cell && p > g_core ||
        !is_core_cell && p > g_edge) {
        return cell->value;
    }



    if (!rule_cell_type(cell)) return cell->value;
    if (!rule_edge_cell(cell)) return cell->value;

    bool is_rule_success = false;
    int new_value = roulette_wheel(cell->transP, g_RND);
    switch (new_value) {
        case 1:
            is_rule_success =
                rule_quantity(cell, 1, g_arable) &&
                rule_suitability(cell, 1, 0.3) &&
                rule_soil_condition(cell) &&
                rule_farming_radius(cell, 40);
            break;
        case 2:
            is_rule_success =
                rule_quantity(cell, 2, g_orchard) &&
                rule_road_access(cell, 500.0);
            break;
        case 3:
            is_rule_success =
                rule_quantity(cell, 3, g_forest);
            break;
        case 5:
            is_rule_success =
                rule_quantity(cell, 5, g_urban) &&
                rule_neighbors_has(cell, 1, 5);
            break;
        case 6:
            is_rule_success =
                rule_quantity(cell, 6, g_rural) &&
                rule_neighbors_has(cell, 1, 6);
            break;
    }

    return is_rule_success ? new_value : cell->value;
}

bool rule_quantity(Cell* cell, int value, int max)
{
    int count = cell->map->counts[value];
    return count < max;
}

bool rule_neighbors_has(Cell* cell, int radius, int value)
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == value) return true;
    }

    return false;
}

bool rule_cell_type(Cell* cell)
{
    return cell->type > 0;
}

bool rule_edge_cell(Cell* cell)
{
    bool is_edge_cell = false;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, g_depth_of_edge);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            is_edge_cell = true;
            break;
        }
    }

    double p = g_RND->nextDouble();
    if (is_edge_cell) {
        if (p <= g_edge) return true;
    } else {
        if (p <= g_core) return true;
    }

    return false;
}

bool rule_soil_condition(Cell* cell)
{
    double soil_depth = g_soil_depth_map.atxy(cell->x, cell->y);
    double slope = g_slope_map.atxy(cell->x, cell->y);

    return soil_depth >= 0.3 && slope < 25;
}

bool rule_farming_radius(Cell* cell, int radius)
{
    return rule_neighbors_has(cell, radius, 6);
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

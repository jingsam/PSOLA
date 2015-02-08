#include "rule.h"
#include "option.h" // g_option.h
#include "tool.h"   // stoi(), stod()


bool rule_neighbors_has(Cell* cell, int radius, int value)
{
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == value) return true;
    }

    return false;
}

bool rule_core_edge(Cell* cell, double p)
{
    int depth_of_edge = stoi(g_option["depth-of-edge"]);
    double g_core = stod(g_option["core"]);
    double g_edge = stod(g_option["edge"]);

    bool is_core_cell = true;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, depth_of_edge);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            is_core_cell = false;
            break;
        }
    }

    if ((is_core_cell && p > g_core) ||
        (!is_core_cell && p > g_edge)) {
        return true;
    }

    return false;
}

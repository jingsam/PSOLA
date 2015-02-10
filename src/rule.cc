#include "rule.h"
#include "option.h" // g_option.h
#include "tool.h"   // stoi(), stod()


int neighbors_has(Cell* cell, int value, int radius)
{
    int count = 0;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == value) count++;
    }

    return count;
}

bool core_edge_operator(Cell* cell, double p)
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

void neighbors_operator(Cell* cell, int level)
{
    int count = cell->transP.size();
    std::vector<int> values(count, 0);

    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, level);
    if (neighbors.size() == 0) return;

    for (int i = 0; i < neighbors.size(); ++i) {
        int value = neighbors.at(i)->value;
        if (value == cell->map->nodata) continue;

        values.at(value - 1)++;
    }

    for (int i = 0; i < values.size(); ++i)
    {
        cell->transP.at(i) += (double)values.at(i) / neighbors.size();
    }

    normalize(cell->transP);
}

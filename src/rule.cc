#include "rule.h"
#include "option.h" // g_option.h
#include "tool.h"   // stoi(), stod(), routtle_wheel()


int neighbors_count(Cell* cell, int value, int radius)
{
    int count = 0;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, radius);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value == value) count++;
    }

    return count;
}

int core_edge_operator(Cell* cell, Random* rnd)
{
    int depth_of_edge = stoi(g_option["depth-of-edge"]);
    double g_core = stod(g_option["core"]);
    double g_edge = stod(g_option["edge"]);

    double r = rnd->nextDouble();
    int value = routtle_wheel(cell->transP, r);

    bool is_core_cell = true;
    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, depth_of_edge);
    for (int i=0; i < neighbors.size(); ++i) {
        if (neighbors.at(i)->value != cell->value) {
            is_core_cell = false;
            break;
        }
    }

    if (is_core_cell && g_core >= r ||
        !is_core_cell && g_edge >=r)
        return value;

    return cell->value;
}

void neighbor_effects(Cell* cell, int level)
{
    int count = cell->transP.size();
    std::vector<int> values(count, 0);

    std::vector<Cell*> neighbors = cell->map->neighbors(cell->x, cell->y, level);
    for (int i = 0; i < neighbors.size(); ++i) {
        int value = neighbors.at(i)->value;
        if (value == cell->map->nodata) continue;

        values.at(value)++;
    }

    for (int i = 0; i < cell->transP.size(); ++i) {
        cell->transP.at(i) += (double)values.at(i) / neighbors.size();
    }

    normalize(cell->transP);
}

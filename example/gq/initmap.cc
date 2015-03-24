#include "../../src/psola.h"
#include "parameter.h"


void undevelop_area(Cell* cell);
void keep_big_village(Cell* cell);
void forest_conservation(Cell* cell);

PlanMap* init_planmap()
{
    int max = stoi(g_option["max"]);

    PlanMap* planmap = new PlanMap(g_land_use_map);
    for (int i = 0; i < planmap->size(); ++i) {
        Cell* cell = planmap->at(i);
        cell->type = 1;
        cell->transP.assign(max, 1.0 / max);

        undevelop_area(cell);
        keep_big_village(cell);
        forest_conservation(cell);
    }

    planmap->updateFitness();

    return planmap;
}

void undevelop_area(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);

    if (land_use == 3 || land_use == 4 || land_use >= 6) {
        cell->value = land_use;
        cell->type = 0;
    }
}

void keep_big_village(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);

    if (land_use == 5) {
            std::vector<Cell*> patch = cell->map->getPatch(cell->x, cell->y);
            if (patch.size() < 16) return;
            for (int i = 0; i < patch.size(); ++i) {
                Cell* mycell = patch.at(i);
                if (g_urban_map.atxy(mycell->x, mycell->y) == 1) continue;
                mycell->value = 5;
                mycell->type = 0;
            }
        }
}

void forest_conservation(Cell* cell)
{
    int land_use = g_land_use_map.atxy(cell->x, cell->y);

    if (land_use == 2) {
        if (neighbors_count(cell, 8, 2) >= 1) {
            cell->value = 2;
            cell->type = 0;
        }
    }
}

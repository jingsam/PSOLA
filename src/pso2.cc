#include "pso.h"
#include "option.h" // g_option
#include "tool.h"   // normalize(), to_string()


int transition(Cell* mycell);

void Particle::updateCurrent(Landscape* gbest)
{
    for (int i = 0; i < current->size(); ++i) {
        Cell* cell = current->at(i);
        Cell* pbest_cell = pbest->at(i);
        Cell* gbest_cell = gbest->at(i);
    }
}

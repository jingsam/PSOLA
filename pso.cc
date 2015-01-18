#include "pso.h"
#include "option.h" // g_option
#include <iostream>

void normalize(std::vector<double> &p);
int transition(Cell* mycell);


void Particle::updateCurrent(PlanMap* gbest)
{
    bool isAsync = g_option["mode"] == "async" ? true ：false;

    double m = this->swarm->momentum;
    double c1 = this->swarm->c1;
    double c2 = this->swarm->c2;
    Random* r1 = this->swarm->r1;
    Random* r2 = this->swarm->r2;
    int xsize = this->current->xsize;
    int ysize = this->current->ysize;
    double nodata = this->current->nodata;

    Map<int> temp(xsize, ysize, nodata, 0);

    for (int i = 0; i < current->size(); ++i) {
        Cell* cell = current->at(i);
        Cell* pbest_cell = pbest->at(i);
        Cell* gbest_cell = gbest->at(i);

        int value = cell->value;
        int pbest_value = pbest_cell->value;
        int gbest_value = gbest_cell->value;

        if (value == nodata) continue;
        if (cell->type == 0) continue;

        // update velocity
        // v(i+1) = m*v(i) + c1*r1*(pbest-x(i)) + c2*r2*(gbest-x(i))
        for (int j = 0; j < cell->transP.size(); ++j) {
            cell->transP.at(j) *= m;
        }
        cell->transP.at(pbest_value - 1) += c1 * r1->nextDouble();
        cell->transP.at(gbest_value - 1) += c2 * r2->nextDouble();
        normalize(cell->transP);

        // update position
        int value = transition(cell);
        if (isAsync) {
            cell->value = value;
        } else {
            temp.at(i) = value;
        }
    }

    this->current->assignValue(temp);
    this->current->updateStats();
    this->updatePbest();
}

#include "pso.h"


void normalize_p(std::vector<double> &p);
int transition(Cell* mycell);


void Particle::updateCurrent(PlanMap* gbest)
{
    double m = this->swarm->momentum;
    double c1 = this->swarm->c1;
    double c2 = this->swarm->c2;
    Random* r1 = this->swarm->r1;
    Random* r2 = this->swarm->r2;
    current->stats->reset();

    for (int i = 0; i < current->size(); ++i) {
        Cell* mycell = current->at(i);
        Cell* pbest_cell = pbest->at(i);
        Cell* gbest_cell = gbest->at(i);

        int myvalue = mycell->value;
        int pbest_value = pbest_cell->value;
        int gbest_value = gbest_cell->value;
		
		if 		(mycell->type == kBackgroundCell) continue;
        else if (mycell->type == kExcludedCell) continue;
        else if (mycell->type == kDeterminedCell) {
            current->stats->counts.at(myvalue - 1)++;
            continue;
        }

        // update velocity
        // v(i+1) = m*v(i) + c1*r1*(pbest-x(i)) + c2*r2*(gbest-x(i))
        for (int j = 0; j < mycell->transP.size(); ++j) {            
			mycell->transP.at(j) *= m;
        }		
		mycell->transP.at(pbest_value - 1) += c1 * r1->nextDouble();
		mycell->transP.at(gbest_value - 1) += c2 * r2->nextDouble();
        normalize_p(mycell->transP);

        // update position
        int value = transition(mycell);
        current->stats->counts.at(value - 1)++;
    }

    this->current->updateFitness();
    this->updatePbest();
}

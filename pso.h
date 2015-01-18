#ifndef PSO_H
#define PSO_H

#include "planmap.h"
#include "random.h"

class Particle;
class Swarm;

class Particle
{
public:
    Particle() {}

    ~Particle() {
        delete current;
        delete pbest;
    }

    void updatePbest() {
        if (current->betterThan(pbest)) {
            pbest->assignValue(current);
        }
    }

    void updateCurrent(PlanMap* gbest);


    PlanMap* current;
    PlanMap* pbest;
    Swarm* swarm;
};


class Swarm : public std::vector<Particle*>
{
public:
    Swarm() {}

    ~Swarm() {
        for (int i = 0; i < this->size(); ++i) {
            delete this->at(i);
        }
        delete r1;
        delete r2;
        delete gbest;
    }

    void updateGbest() {
        PlanMap* best = this->at(0)->current;
        for (int i = 0; i < this->size(); ++i) {
            PlanMap* map = this->at(i)->current;
            if (map->betterThan(best)) {
                best = map;
            }
        }

        gbest->assignValue(best);
    }

    void updateParticles() {
        for (int i = 0; i < this->size(); ++i) {
            this->at(i)->updateCurrent(this->gbest);
        }

        this->updateGbest();
    }

    double momentum, c1, c2;
    Random *r1, *r2;
    PlanMap* gbest;
};


#endif

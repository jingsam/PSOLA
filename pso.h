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
        if (current->betterThan( pbest )) {
            pbest->equalTo(current);
        }
    }

    void updateCurrent(PlanMap* gbest);


    PlanMap* current;
    PlanMap* pbest;
    Swarm* swarm;
};


class Swarm
{
public:
    Swarm() {}

    ~Swarm() {
        for (int i = 0; i < particles.size(); ++i) {
            delete particles.at(i);
        }
        delete r1;
        delete r2;
        delete gbest;
    }

    void updateGbest() {
        PlanMap* best = particles[0]->current;
        for (int i = 0; i < particles.size(); ++i) {
            PlanMap* map = particles.at(i)->current;
            if (map->betterThan( best )) {
                best = map;
            }
        }

        gbest->equalTo(best);
    }

    void updateParticles() {
        for (int i = 0; i < particles.size(); ++i) {
            particles.at(i)->updateCurrent( this->gbest );
        }

        this->updateGbest();
    }

    double momentum, c1, c2;
    Random *r1, *r2;
    PlanMap* gbest;
    std::vector<Particle*> particles;
};


#endif

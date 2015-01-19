#include "init.h"
#include <string>   // stoi(), stod()
#include "option.h" // g_option
#include "parameter.h"  // g_RND, g_land_use_map


PlanMap* init_planmap()
{
    int max = stoi(g_option["max"]);

    PlanMap* planmap = new PlanMap(g_land_use_map);
    for (int i = 0; i < planmap->size(); ++i) {
        Cell* cell = planmap->at(i);
        cell->type = 1;
        cell->transP.assign(max, 1.0 / max);
    }
    planmap->updateStats();

    return planmap;
}

Particle* init_particle()
{
    Particle* particle = new Particle();
    particle->current = init_planmap();
    particle->pbest = particle->current->clone();

    return particle;
}

Swarm* init_swarm(int size, int id)
{
    double momentum = stod(g_option["momentum"]);
    double c1 = stod(g_option["c1"]);
    double c2 = stod(g_option["c2"]);
    int r1 = stoi(g_option["r1"]);
    int r2 = stoi(g_option["r2"]);
    int seed = stoi(g_option["seed"]);

    Swarm* swarm = new Swarm();
    swarm->momentum = momentum;
    swarm->c1 = c1;
    swarm->c2 = c2;
    swarm->r1 = new Random(r1 + id);
    swarm->r2 = new Random(r2 + id);
    g_RND->srand(seed + id);

    for (int i = 0; i < size; ++i) {
        Particle* particle = init_particle();
        particle->swarm = swarm;
        swarm->push_back(particle);
    }

    swarm->gbest = swarm->at(0)->current->clone();

    return swarm;
}

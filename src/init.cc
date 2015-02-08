#include "init.h"
#include "option.h" // g_option, g_rnd, g_land_use_map
#include "tool.h"   // stoi(), stod()

Map<int> init_map();

PlanMap* init_planmap()
{
    int max = stoi(g_option["max"]);
    Map<int> map = init_map();

    PlanMap* planmap = new PlanMap(map);
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

Swarm* init_swarm(int size)
{
    double momentum = stod(g_option["momentum"]);
    double c1 = stod(g_option["c1"]);
    double c2 = stod(g_option["c2"]);
    int r1 = stoi(g_option["r1"]);
    int r2 = stoi(g_option["r2"]);

    Swarm* swarm = new Swarm();
    swarm->momentum = momentum;
    swarm->c1 = c1;
    swarm->c2 = c2;
    swarm->r1 = new Random(r1);
    swarm->r2 = new Random(r2);

    for (int i = 0; i < size; ++i) {
        Particle* particle = init_particle();
        particle->swarm = swarm;
        swarm->push_back(particle);
    }

    swarm->gbest = swarm->at(0)->current->clone();

    return swarm;
}

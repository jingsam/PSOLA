#include "init.h"
#include <string>   // stoi(), stod()
#include "option.h"




PlanMap* init_map()
{
    int max = std::stoi(g_option["max"]);

    PlanMap* map = new PlanMap(g_land_use_map);
    for (int i = 0; i < map.size(); ++i) {
        Cell* cell = map->at(i);
        cell->type = 1;
        cell->transP.assign(max, 1.0 / max);
    }
    map->updateStats();

    return map;
}

Particle* init_particle()
{
    Particle* particle = new Particle();
    particle->current = init_map();
    particle->pbest = particle->current->clone();

    return particle;
}

Swarm* init_swarm(int size, int id)
{
    double momentum = std::stod(g_option["momentum"]);
    double c1 = std::stod(g_option["c1"]);
    double c2 = std::stod(g_option["c2"]);
    int r1 = std::stoi(g_option["r1"]);
    int r2 = std::stoi(g_option["r2"]);
    int seed = std::stoi(g_option["seed"]);

    Swarm* swarm = new Swarm();
    swarm->momentum = momentum;
    swarm->c1 = c1;
    swarm->c2 = c2;
    swarm->r1 = new Random(r1 + id);
    swarm->r2 = new Random(r2 + id);
    g_RND = new Random(seed + id);

    for (int i = 0; i < size; ++i) {
        Particle* particle = init_particle();
        particle->swarm = swarm;
        swarm->push_back(particle);
    }

    swarm->gbest = swarm->at(0)->current->clone();
    swarm->updateGbest();

    return swarm;
}

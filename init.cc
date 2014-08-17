#include <cstdlib>
#include "init.h"
#include "option.h"

int classify_land_use(Cell* cell);
void set_cell_type(Cell* cell);


Cell* init_cell(int x, int y)
{
    Cell* cell = new Cell();
    cell->x = x;
    cell->y = y;
	cell->value = classify_land_use(cell);
    cell->transP.assign(g_max, 1.0 / g_max);

    set_cell_type(cell);

    return cell;
}

PlanMap* init_map()
{
    PlanMap* map = new PlanMap();
    map->xsize = g_xsize;
    map->ysize = g_ysize;
    map->stats = new Stats();
	map->stats->counts.assign(g_max, 0);
    for (int y = 0; y < map->ysize; ++y) {
        for (int x = 0; x < map->xsize; ++x) {	
            Cell* cell = init_cell(x, y);
            cell->map = map;
            map->push_back(cell);
        }
    }

    map->updateFitness();

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
    RND = new Random( g_seed + id );

    Swarm* swarm = new Swarm();
    swarm->momentum = g_momentum;
    swarm->c1 = g_c1;
    swarm->c2 = g_c2;
    swarm->r1 = new Random( g_r1 + id );
    swarm->r2 = new Random( g_r2 + id );

    for (int i = 0; i < size; ++i) {
        Particle* particle = init_particle();
        particle->swarm = swarm;
        swarm->particles.push_back(particle);
    }

    swarm->gbest = swarm->particles[0]->current->clone();
    swarm->updateGbest();

    return swarm;
}

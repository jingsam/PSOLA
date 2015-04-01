#include <cstdlib>  // calloc(), free(), exit();
#include <cstdio>   // printf(), NULL
#include <cstring>  // memcpy()
#include <string>
#include <fstream>  // ofstream
#include "mpi.h"
#include "../../src/psola.h"
#include "parameter.h"


void func(int *in, int *inout, int *len, MPI_Datatype *ctype);

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank==0) {
        std::printf("\n--------------Parse configuration-------------\n");
    }

    double t1 = MPI_Wtime();
    parse_option(argc, argv);
    set_parameter();

    int population = stoi(g_option["population"]);
    int interval = stoi(g_option["interval"]);
    int generation = stoi(g_option["generation"]);
    std::string out_dir = g_option["output"];
    std::string land_use_map = g_option["land-use-map"];
    int map_size = g_land_use_map.size();

    std::ofstream ofs;
    if (rank==0) {
        std::string cmd = "mkdir -p " + out_dir;
        system(cmd.c_str());
        std::string log = out_dir + "/log.csv";
        ofs.open(log.c_str(), std::ofstream::out | std::ofstream::trunc);
        ofs << "generation" << ","
            << "fitness" << ","
            << "social-benefit" << ","
            << "economic-benefit" << ","
            << "ecological-benefit" << std::endl;

    }
    double t2 = MPI_Wtime();


    if (rank==0) {
        show_option();
        std::printf("\nAccomplished: %.2f S\n", t2 - t1);
        std::printf("\n--------------Initialize components-----------\n");
    }

    double t3 = MPI_Wtime();
    Swarm* swarm = init_swarm((population + size - 1) / size);
    swarm->r1->srand(rank);
    swarm->r2->srand(rank);
    g_rnd->srand(rank);
    double t4 = MPI_Wtime();


    if (rank==0) {
        std::printf("\nAccomplished: %.2f S\n", t4 - t3);
        std::printf("\n--------------Start optimization--------------\n");
        std::printf("\ngen  fitness   social    economic  ecological\n");
    }

    double t5 = MPI_Wtime();
    MPI_Datatype ctype;
    MPI_Type_contiguous(1 + map_size, MPI_INT, &ctype);
    MPI_Type_commit(&ctype);
    MPI_Op op;
    MPI_Op_create((MPI_User_function*)func, 1, &op);

    void* sendbuff = calloc(1 + map_size, sizeof(int));
    void* recvbuff = calloc(1 + map_size, sizeof(int));
    if (sendbuff == NULL || recvbuff == NULL)
    {
        std::printf("[ERROR]Failed to allocate memory!");
        std::exit(1);
    }

    for (int i = 1; i <= generation; ++i) {
        if (rank==0) {
            std::map<std::string, double> stats = swarm->gbest->stats;
            double fitness = stats["fitness"];
            double social = stats["social-benefit"];
            double economic = stats["economic-benefit"];
            double ecological = stats["ecological-benefit"];

            ofs << i << ","
                << fitness << ","
                << social << ","
                << economic << ","
                << ecological << std::endl;

            std::printf("%3d  %.6f  %.6f  %.6f  %.6f\n",
                i, fitness, social, economic, ecological);

            if (i == 1 || interval != 0 && (i % interval) == 0) {
                std::string file = out_dir + "/" + to_string(i) + ".tif";
                writeRaster(swarm->gbest->getDataMap(), file.c_str(), land_use_map.c_str());
            }
        }

        swarm->updateParticles();

        ((float*)sendbuff)[0] = (float)swarm->gbest->stats["fitness"];
        for (int j = 0; j < map_size; ++j) {
            ((int*)sendbuff)[j+1] = (int)swarm->gbest->at(j)->value;
        }

        MPI_Allreduce(sendbuff, recvbuff, 1, ctype, op, MPI_COMM_WORLD);

        swarm->gbest->stats["fitness"] = ((float*)recvbuff)[0];
        for (int k = 0; k < map_size; ++k) {
            swarm->gbest->at(k)->value = ((int*)recvbuff)[k+1];
        }
        swarm->gbest->updateFitness();

        MPI_Barrier(MPI_COMM_WORLD);
    }
    double t6 = MPI_Wtime();


    if (rank==0) {
        std::printf("\nAccomplished: %.2f S\n", t6 - t5);
        std::printf("\n--------------Output results------------------\n");

        double t7 = MPI_Wtime();
        std::string output = out_dir + "/result.tif";
        writeRaster(swarm->gbest->getDataMap(), output.c_str(), land_use_map.c_str());
        double t8 = MPI_Wtime();

        std::printf("\nAccomplished: %.2f S\n", t8 - t7);
        std::printf("\n----------------------------------------------\n");
        std::printf("\nTotal: %.2f S\n", t8 - t1);
    }

    ofs.close();
    free(sendbuff);
    free(recvbuff);
    MPI_Type_free(&ctype);
    MPI_Op_free(&op);
    MPI_Finalize();
    return 0;
}

void func(int *in, int *inout, int *len, MPI_Datatype *ctype)
{
    int size;
    MPI_Type_size(*ctype, &size);
    int length = size / sizeof(int);

    for (int i = 0; i < *len; ++i) {
        float fitness1 = ((float*)in)[0];
        float fitness2 = ((float*)inout)[0];
        if (fitness1 > fitness2) memcpy(inout, in, size);

        in += length;
        inout += length;
    }
}

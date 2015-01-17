#include <cstdlib>  // calloc(), free(), exit();
#include <cstdio>   // printf(), sprintf, stderr, NULL
#include <cstring>  // memcpy()
#include <string>   // to_string()
#include <sstream>  // ostringstream
#include "mpi.h"
#include "psola.h"

void new_type(MPI_Datatype* ctype);
void myOp(void* in, void* inout, int *len, MPI_Datatype* ctype);
void new_op(MPI_Op* op);


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
    if (rank==0) {
        std::string cmd = "mkdir -p " + g_option["output"];
        system(cmd.c_str());
    }
    double t2 = MPI_Wtime();


    if (rank==0) {
        show_option();
        std::printf("\nAccomplished: %.2f S\n", t2 - t1);
        std::printf("\n--------------Initialize components-----------\n");
    }

    double t3 = MPI_Wtime();
    int population = std::stoi(g_option["population"]);
    Swarm* swarm = init_swarm((population + size - 1) / size, rank);
    double t4 = MPI_Wtime();


    if (rank==0) {
        std::printf("\nAccomplished: %.2f S\n", t4 - t3);
        std::printf("\n--------------Start optimization--------------\n");
    }

    double t5 = MPI_Wtime();
    MPI_Datatype ctype;
    new_type(&ctype);
    MPI_Op myop;
    new_op(&myop);

    int map_size = g_land_use_map.size();
    void* sendbuff = calloc(1 + map_size, sizeof(int));
    void* recvbuff = calloc(1 + map_size, sizeof(int));
    if (sendbuff == NULL || recvbuff == NULL)
    {
        std::sprintf(stderr, "[ERROR]Failed to allocate memory!");
        std::exit(1);
    }

    for (int i = 0; i <= g_generation; ++i) {
        swarm->updateParticles();

        ((float*)sendbuff)[0] = (float)swarm->gbest->stats["fitness"];
        for (int j = 0; j < map_size; ++j) {
            ((int*)sendbuff)[j+1] = (int)swarm->gbest->at(j)->value;
        }

        MPI_Allreduce(sendbuff, recvbuff, 1, ctype, myop, MPI_COMM_WORLD);

        swarm->gbest->stats["fitness"] = ((float*)recvbuff)[0];
        for (int k = 0; k < map_size; ++k) {
            swarm->gbest->at(k)->value = ((int*)recvbuff)[k+1];
        }

        if (rank==0) {
            if (g_interval != 0 && (i % g_interval) == 0) {
                std::string file = g_option["output"] + "/"
                    + std::to_string(i) + ".tif";
                writeRaster(swarm->gbest->getDataMap(), file.c_str(),
                    g_option["land-use-map"].c_str());
            }

            logStats(swarm->gbest->stats);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }
    double t6 = MPI_Wtime();


    if (rank==0) {
        std::printf("Accomplished: %.2f S\n", t6 - t5);
        std::printf("\n--------------Output final results------------\n");

        double t7 = MPI_Wtime();
        std::string log = g_output + "/log.csv";


        std::string output = g_output + "/result.tif";
        writeRaster(swarm->gbest->getDataMap(), output.c_str()
            g_option["land-use-map"].c_str());
        double t8 = MPI_Wtime();

        std::printf("\nAccomplished: %.2f S\n", t8 - t7);
        std::printf("\n----------------------------------------------\n");
        std::printf("\nTotal: %.2f S\n", t8 - t1);
    }


    free(sendbuff);
    free(recvbuff);
    MPI_Type_free(&ctype);
    MPI_Op_free(&myop);
    MPI_Finalize();
    return 0;
}

void new_type(MPI_Datatype* ctype)
{
    int map_size = g_land_use_map.size();

    const int count = 2;
    int blockcounts[] = { 1, map_size };
    MPI_Aint offsets[] = { 0, sizeof(float) };
    MPI_Datatype oldtypes[] = { MPI_FLOAT, MPI_INT };

    MPI_Type_create_struct(count, blockcounts, offsets, oldtypes, ctype);
    MPI_Type_commit(ctype);
}

void myOp(void* in, void* inout, int* len, MPI_Datatype* ctype)
{
    float fitness1 = ((float*)in)[0];
    float fitness2 = ((float*)inout)[0];

    if (fitness1 > fitness2) {
        memcpy(inout, in, *len);
    }
}

void new_op(MPI_Op* op)
{
    MPI_User_function* func = (MPI_User_function*)myOp;
    int commute = 1;

    MPI_Op_create(func, commute, op);
}

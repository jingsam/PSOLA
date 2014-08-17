#include <cstdlib>  // calloc()
#include <cstdio>   // printf()
#include <ctime>    // clock()
#include <cstring>  // memcpy()
#include "mpi.h"
#include "psola.h"

void new_type(MPI_Datatype* ctype);
void myOp(void* in, void* inout, int *len, MPI_Datatype* ctype);
void new_op(MPI_Op* op);


int main( int argc, char *argv[] )
{
    int size, rank;
    // MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

if (rank==0) {
    std::printf("\n--------------Parse configuration-------------\n");
}
    clock_t t1 = clock();

    parse_option(argc, argv);

    clock_t t2 = clock();
if (rank==0) {
    std::printf("\nAccomplished: %.2f S\n",
        (double)(t2-t1) / CLOCKS_PER_SEC);
}


if (rank==0) {
    std::printf("\n--------------Initialize components-----------\n");
}
    clock_t t3 = clock();

    Swarm* swarm = init_swarm( (g_size + size - 1) / size, rank );
if (rank==0) init_output();

    clock_t t4 = clock();
if (rank==0) {
    std::printf("\nAccomplished: %.2f S\n",
        (double)(t4-t3) / CLOCKS_PER_SEC);
}


if (rank==0) {
    std::printf("\n--------------Start optimization--------------\n");
}
    clock_t t5 = clock();

    MPI_Datatype ctype;
    new_type( &ctype );

    MPI_Op myop;
    new_op( &myop );

    void* sendbuff = calloc( 1 + g_xsize * g_ysize, sizeof(int) );
    void* recvbuff = calloc( 1 + g_xsize * g_ysize, sizeof(int) );
    tinyxml2::XMLDocument* doc = createLogDocument();

    for (int i = 0; i <= g_generation; ++i) {
        swarm->updateParticles();

        ((float*)sendbuff)[0] = (float)swarm->gbest->stats->fitness;
        for (int j = 0; j < g_xsize * g_ysize; ++j) {
            ((int*)sendbuff)[j+1] = (int)swarm->gbest->at(j)->value;
        }

        MPI_Allreduce( sendbuff, recvbuff, 1, ctype, myop, MPI_COMM_WORLD );

        swarm->gbest->stats->fitness = ((float*)recvbuff)[0];
        for (int k = 0; k < g_xsize * g_ysize; ++k) {
            swarm->gbest->at(k)->value = ((int*)recvbuff)[k+1];
        }

if (rank==0) std::printf("%d, %f\n", i, swarm->gbest->stats->fitness);

if (rank==0) {
        // ouptut middle result
        if (g_interval != 0 && (i % g_interval) == 0) {
            char buffer[50];
            std::sprintf(buffer, "%d", i);
            std::string filename = g_output + "/" + g_prefix + buffer + ".tif";
            outputImage(swarm->gbest->getDataMap(), filename.c_str());
            logStatus(doc, i, swarm->gbest->stats->fitness,
                (g_prefix + buffer + ".tif").c_str());
        } else {
            logStatus(doc, i, swarm->gbest->stats->fitness);
        }
}

        MPI_Barrier( MPI_COMM_WORLD );
    }

    clock_t t6 = clock();
if (rank==0) {
    std::printf("Accomplished: %.2f S\n",
        (double)(t6-t5) / CLOCKS_PER_SEC);
}

if (rank==0) {
    std::printf("\n--------------Output final results------------\n");
    clock_t t7 = clock();

    std::string log = g_output + "/" + g_prefix + "log.xml";
    if (doc->SaveFile(log.c_str())) {
        std::printf("Failed save log to %s", log.c_str());
    }

    std::string output = g_output + "/" + g_prefix + "result.tif";
    outputImage(swarm->gbest->getDataMap(), output.c_str());

    clock_t t8 = clock();
    std::printf("\nAccomplished: %.2f S\n",
        (double)(t8-t7) / CLOCKS_PER_SEC);

	std::printf("\n----------------------------------------------\n");
	std::printf("\nTotal: %.2f S\n",
        (double)(t8-t1) / CLOCKS_PER_SEC);
}

	clean_option();
    MPI_Type_free( &ctype );
    MPI_Op_free( &myop );
    MPI_Finalize();
    return 0;

}

// typedef struct {
//     float fitness;
//     int map[g_xsize * g_ysize];
// } Message;

void new_type(MPI_Datatype* ctype)
{
    const int count = 2;
    int blockcounts[] = { 1, g_xsize * g_ysize };
    MPI_Aint offsets[] = { 0, sizeof(float) };
    MPI_Datatype oldtypes[] = { MPI_FLOAT, MPI_INT };

    MPI_Type_create_struct( count, blockcounts, offsets, oldtypes, ctype );
    MPI_Type_commit( ctype );
}

void myOp(void* in, void* inout, int *len, MPI_Datatype* ctype)
{
    float fitness1 = ((float*)in)[0];
    float fitness2 = ((float*)inout)[0];

    if (fitness1 > fitness2) {
        memcpy( inout, in, *len );
    }
}

void new_op(MPI_Op* op)
{
    MPI_User_function* func = (MPI_User_function*)myOp;
    int commute = 1;

    MPI_Op_create( func, commute, op );
}

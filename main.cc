#include <cstdlib>  // calloc()
#include <cstdio>   // printf()
#include <ctime>    // clock()
#include <cstring>  // memcpy()
#include <sstream> // ostringstream
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
    
    double t1 = MPI_Wtime();
    parse_option(argc, argv);
    double t2 = MPI_Wtime();
    
    if (rank==0) {
        std::printf("\nAccomplished: %.2f S\n", t2 - t1);
        std::printf("\n--------------Initialize components-----------\n");
    }

    double t3 = MPI_Wtime();
    Swarm* swarm = init_swarm( (g_size + size - 1) / size, rank );
    if (rank==0) init_output();
    double t4 = MPI_Wtime();
    
    if (rank==0) {
        std::printf("\nAccomplished: %.2f S\n", t4 - t3);
        std::printf("\n--------------Start optimization--------------\n");
    }


    double t5 = MPI_Wtime();
    MPI_Datatype ctype;
    new_type( &ctype );

    MPI_Op myop;
    new_op( &myop );

    void* sendbuff = calloc( 1 + g_xsize * g_ysize, sizeof(int) );
    void* recvbuff = calloc( 1 + g_xsize * g_ysize, sizeof(int) );
    tinyxml2::XMLDocument* doc = createLogDocument();

    for (int i = 0; i <= g_generation; ++i) {
        swarm->updateParticles();

        ((float*)sendbuff)[0] = (float)swarm->gbest->fitness;
        for (int j = 0; j < g_xsize * g_ysize; ++j) {
            ((int*)sendbuff)[j+1] = (int)swarm->gbest->at(j)->value;
        }

        MPI_Allreduce( sendbuff, recvbuff, 1, ctype, myop, MPI_COMM_WORLD );

        swarm->gbest->fitness = ((float*)recvbuff)[0];
        for (int k = 0; k < g_xsize * g_ysize; ++k) {
            swarm->gbest->at(k)->value = ((int*)recvbuff)[k+1];
        }

        if (rank==0) {
    	    std::printf("%d, %f\n", i, swarm->gbest->fitness);
    	    // ouptut middle result
            if (g_interval != 0 && (i % g_interval) == 0) {
                std::ostringstream oss;
                oss << i << ".tif" << std::endl;
                std::string file = oss.str();
                outputImage(swarm->gbest->getDataMap(), (g_output + file).c_str());
                logStatus(doc, i, swarm->gbest->fitness, file.c_str());
            } else {
                logStatus(doc, i, swarm->gbest->fitness);
            }
        }

        MPI_Barrier( MPI_COMM_WORLD );
    }
    double t6 = MPI_Wtime();
    
    if (rank==0) {
        std::printf("Accomplished: %.2f S\n", t6 - t5);
        std::printf("\n--------------Output final results------------\n");

        double t7 = MPI_Wtime();
        std::string log = g_output + "/log.xml";
        if (doc->SaveFile(log.c_str())) {
            std::printf("Failed save log to %s", log.c_str());
        }

        std::string output = g_output + "/result.tif";
        outputImage(swarm->gbest->getDataMap(), output.c_str());
        double t8 = MPI_Wtime();
    
        std::printf("\nAccomplished: %.2f S\n", t8 - t7);
        std::printf("\n----------------------------------------------\n");
        std::printf("\nTotal: %.2f S\n", t8 - t1);
    }

    MPI_Type_free( &ctype );
    MPI_Op_free( &myop );
    MPI_Finalize();
    return 0;
}

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

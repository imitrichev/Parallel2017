#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_SPAWNS 2

int main( int argc, char *argv[] )
{
    int np = NUM_SPAWNS;
    int errcodes[NUM_SPAWNS];
    MPI_Init( &argc, &argv );
    MPI_Comm parentcomm, intercomm;

    MPI_Comm_get_parent( &parentcomm );
    if (parentcomm == MPI_COMM_NULL)
    {
	    char command[] = "a.out";
	    char* my_argv[] = {"-a", "abc", NULL};
	    /* Create 2 more processes - this example must be called spawn_example.exe for this to work. */
	    MPI_Comm_spawn( command, my_argv, np, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, errcodes);
    }
    else
    {
	int index;
	for(index = 0; index < argc; index++)
		printf("The %d is %s\n",index,argv[index]);
    }
    fflush(stdout);
    MPI_Finalize();
    return 0;
}
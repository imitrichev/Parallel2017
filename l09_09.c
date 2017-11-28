#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// compile this program both as program1 and program2
// mpicc l09_09.c -o program1 && mpicc l09_09.c -o program2 && mpirun -np 1 ./program1

//2 child processes totally
//Process 0 made a broadcast
//Process 0 in parentcomm
//Process 0 received 567
//Process 1 in parentcomm
//Process 1 received 567

#define NUM_SPAWNS 2

int main( int argc, char *argv[] )
{
    int errcodes[NUM_SPAWNS];
    MPI_Init( &argc, &argv );
    MPI_Comm parentcomm, intercomm;

    MPI_Comm_get_parent( &parentcomm );
    if (parentcomm == MPI_COMM_NULL)
    {
	char *array_of_commands[2] = {"program1", "program2"};
	char **array_of_argv[2];
	char *argv0[] = {(char *)0};
	char *argv1[] = {(char *)0};
	array_of_argv[0] = argv0;
	array_of_argv[1] = argv1;
	MPI_Info ar_info[2] = {MPI_INFO_NULL, MPI_INFO_NULL};
	// by 1 process for both commands
	int np[2] = {1,1};

        MPI_Comm_spawn_multiple(2, array_of_commands, array_of_argv, np, ar_info, 0, MPI_COMM_SELF, &intercomm, errcodes);
	int rank;
	int data[1];
	data[0]=567;
	int size;
	MPI_Comm_remote_size(intercomm, &size);
	printf("%d child processes totally\n",size);
	// one should use MPI_ROOT for the process in first (parent) group
	// for sending process and MPI_PROC_NULL for other. All process in the
	// second group use root number in the first group as root parameter
	// (from MPI v2.0 docs)
	MPI_Bcast(data, 1, MPI_INT, MPI_ROOT, intercomm);
	MPI_Comm_rank(intercomm, &rank);
	printf("Process %d made a broadcast\n",rank);
    }
    else
    {
	int rank;
	int recvbuf[1];
	MPI_Comm_rank(parentcomm, &rank);
	printf("Process %d in parentcomm\n",rank);

	// spawned processes communicate in their MPI_COMM_WORLD
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// they can communicate with parent via parent communicator,

	MPI_Bcast(recvbuf, 1, MPI_INT, 0, parentcomm);
	printf("Process %d received %d\n",rank,recvbuf[0]);
    }
    fflush(stdout);
    MPI_Finalize();
    return 0;
}
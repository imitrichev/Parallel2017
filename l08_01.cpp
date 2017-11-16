#include "mpi.h"
#include <stdio.h>
using namespace std;

int main(int argc, char **argv)
{
	int size, rank, a, b;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	a = rank;
	b=-1;
	if (( rank%2 ) == 0){
		if (rank < size - 1)
		MPI_Send(&a, 1, MPI_INT, rank+1, 5, MPI_COMM_WORLD);
	}
	else
		MPI_Recv(&b, 1, MPI_INT, rank-1, 5, MPI_COMM_WORLD, &status);
	cout << " process " << rank << " a = " << a << " b = " << b << endl;
	MPI_Finalize();
}
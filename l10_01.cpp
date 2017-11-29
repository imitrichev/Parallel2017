#include <iostream>
#include "mpi.h"
#define N 64
#define MAXPROC 4
void slave(double *a, int n, int rank)
{
	for (int i=0; i<n; i++)
		a[i]=i+rank;
}

void master(double* a, int num, int portion)
{
	for (int i=0; i<portion; i++)
		std::cout<<a[num*portion+i]<<std::endl;
}

int main(int argc, char **argv)
{
   int rank, size, source, d;
   MPI_Request req[MAXPROC-1];
   MPI_Status statuses[MAXPROC-1];
   double a[N];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   int slaves= size - 1;
   double* b = new double [N/size];

   MPI_Scatter(a, N/size, MPI_DOUBLE, b,
            N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

if(rank!=0)
      {
         slave((double*)b, N/size, rank);
	 d=1; //send some flag
	 MPI_Send(&d, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
	MPI_Gather(b, N/size, MPI_DOUBLE, a, N/size, MPI_DOUBLE, 0,
           MPI_COMM_WORLD);
      }
      else{
         for(int i = 1; i<size; i++)
            MPI_Irecv(&d, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &req[i-1]);

        MPI_Waitall(size-1, req, statuses);
	// to make Gather and Scatter work, master should work as slave
	slave((double*)b, N/size, rank);
	MPI_Gather(b, N/size, MPI_DOUBLE, a, N/size, MPI_DOUBLE, 0,
           MPI_COMM_WORLD);

            for (int i = 0; i< size-1; i++){
               source = statuses[i].MPI_SOURCE;
               master(a,source,N/size);
	    master(a,0,N/size); //print own part of array
            }
      }

   delete [] b;
   MPI_Finalize();
}
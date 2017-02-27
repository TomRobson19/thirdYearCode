#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int * a;
	int * b;
	int * c;
	// arrays a and b
	int total_proc;
	// total nuber of processes
	int rank;
	// rank of each process
	int T;
	// total number of test cases
	long long int n_per_proc;
	// elements per process

	long long int i, n;
	MPI_Status status;

	// Initialization of MPI environment
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &total_proc);
	//Now you know the total number of processes running in parallel
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	//Now you know the rank of the current process
    int * ap;
	int * bp;
	int * cp;
	if (rank == 0) //you choose process rank 0 to be your root which will be used to perform input output.
	{
    	scanf ("%d", &T);
    	// you get the total number of test cases
    	MPI_Bcast (&T, 1, MPI_INT, 0, MPI_COMM_WORLD);
    	// Then you broadcast it to every process out there
    	while (T--)
    	{  	
    		scanf ("%lld", &n);
			a = (int *) malloc(sizeof(int)*n);
			b = (int *) malloc(sizeof(int)*n);
			c = (int *) malloc(sizeof(int)*n);
        	MPI_Bcast (&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
        	n_per_proc = n/total_proc;

			for(i=0;i<n;i++)
			{
				scanf ("%d", &a[i]);
			}
			
			for(i=0;i<n;i++)
			{
			    scanf ("%d", &b[i]);
			}
			if(n%total_proc != 0)
			{
		    	n_per_proc+=1;
		    	for(i=0;i<(n_per_proc*total_proc - n);i++)
		    	{
		    		a[n+i] = 0;
		    		b[n+i] = 0;
		    	}
			} // to divide data evenly by the number of processors 
			ap = (int *) malloc(sizeof(int)*n_per_proc);
			bp = (int *) malloc(sizeof(int)*n_per_proc);
			cp = (int *) malloc(sizeof(int)*n_per_proc);

			MPI_Bcast (&n_per_proc, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
			//Broadcast element per process
			MPI_Scatter(a, n_per_proc, MPI_INT, ap, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//scattering array a  
			MPI_Scatter(b, n_per_proc, MPI_INT, bp, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//scattering array b	

			for(i=0;i<n_per_proc;i++)
			{
				cp[i] = ap[i]+bp[i];
			}
			MPI_Gather(cp, n_per_proc, MPI_INT, c, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//gathering array c
			for(i=0;i<n;i++)
			{
				printf ("%d ", c[i]);
			}
			printf ("\n");
		}
    }
	else
	{ // Non-master tasks
		// Receive T
		MPI_Bcast (&T, 1, MPI_INT, 0, MPI_COMM_WORLD);
		//printf("%d from process %d\n",T,rank);
		while (T--)
		{
			MPI_Bcast (&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast (&n_per_proc, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
			ap = (int *) malloc(sizeof(int)*n_per_proc);
			bp = (int *) malloc(sizeof(int)*n_per_proc);
			cp = (int *) malloc(sizeof(int)*n_per_proc);
			MPI_Scatter(a, n_per_proc, MPI_INT, ap, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//Recieving Scattered a
			MPI_Scatter(b, n_per_proc, MPI_INT, bp, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//Recieving Scattered b
			for(i=0;i<n_per_proc;i++)
			{
				cp[i] = ap[i]+bp[i];
				//printf("%d from %d\n",ap[i],rank);
				//printf("%d from %d\n",bp[i],rank);
			}
			MPI_Gather(cp, n_per_proc, MPI_INT, c, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			//To initiate the gathering of computed part of array C
        }
	}
	MPI_Finalize();
	//Terminate MPI Environment
	return 0;
}
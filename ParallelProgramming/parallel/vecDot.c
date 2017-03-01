//mpicc -g3 vecDot.c -o vecDot
//mpirun -n 4 ./vecDot
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	srand(0);
    if (argc < 2) {
        printf("Syntax: test <size of vector>\n");
        return 1;
    }
    int n = atoi(argv[1]);
	int * a;
	int * b;
	int * c;
	int sum = 0;
	// arrays a and b
	int total_proc;
	// total nuber of processes
	int rank;
	// rank of each process
	int n_per_proc;
	// elements allocated per process
	int i;
	MPI_Status status;
	//Initialization of MPI environment
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &total_proc);
	//total number of processes running in parallel
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	//rank of the current process
	int * ap;
	int * bp;
	int * cp;
	if (rank == 0)
	{
		a = (int *) malloc(sizeof(int)*n);
		b = (int *) malloc(sizeof(int)*n);
		c = (int *) malloc(sizeof(int)*n);

    	MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    	n_per_proc = n/total_proc;

		for(i=0;i<n;i++)
		{
			a[i] = rand() % 1000;
			b[i] = rand() % 1000;
		}
		printf("Vector1 :\n");
		for(i=0;i<n;i++)
		{
			printf ("%d\n", a[i]);
		}	
		printf("Vector2 :\n");
		for(i=0;i<n;i++)
		{
			printf ("%d\n", b[i]);
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

		MPI_Bcast (&n_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
		//Broadcast element per process
		MPI_Scatter(a, n_per_proc, MPI_INT, ap, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
		//scattering array a  
		MPI_Scatter(b, n_per_proc, MPI_INT, bp, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
		//scattering array b	

		for(i=0;i<n_per_proc;i++)
		{
			cp[i] = ap[i]*bp[i];
		}
		MPI_Gather(cp, n_per_proc, MPI_INT, c, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		//MPI_Reduce(&cp, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		//gathering array c
		for(i=0;i<n;i++)
		{
			sum += c[i];
		}		
		printf("Dot Product :\n");
		printf("%d\n", sum);
    }
	else
	{ // Non-master tasks
		MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast (&n_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
		ap = (int *) malloc(sizeof(int)*n_per_proc);
		bp = (int *) malloc(sizeof(int)*n_per_proc);
		cp = (int *) malloc(sizeof(int)*n_per_proc);
		MPI_Scatter(a, n_per_proc, MPI_INT, ap, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
		//Recieving Scattered a
		MPI_Scatter(b, n_per_proc, MPI_INT, bp, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
		//Recieving Scattered b
		//int localSum = 0;
		for(i=0;i<n_per_proc;i++)
		{
			cp[i] = ap[i]*bp[i];
			//localSum += cp[i];
		}
		MPI_Gather(cp, n_per_proc, MPI_INT, c, n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
		//MPI_Reduce(&localSum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	}
	MPI_Finalize();
	return 0;
}
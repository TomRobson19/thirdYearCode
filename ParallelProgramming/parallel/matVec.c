//mpicc -g3 matVec.c -o matVec
//mpirun -n 4 ./matVec
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define AROW 3
#define ACOL 5

/* Process mapping function */
int proc_map(int i, int size)
{
    size = size - 1;
    int r = AROW/size;
    int proc = i / r;
    return proc + 1;
}

int main(int argc, char** argv)
{
	srand(0);
    int size, rank;
    MPI_Status Stat;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    if (rank == 0)
    {
        double a[AROW][ACOL];
        double b[ACOL];
        double c[AROW];
 
        /* Generating Random Values for A & B Array*/
        for (int i=0;i<AROW;i++)
        {
            for (int j=0;j<ACOL;j++)
            {
                if (i==0) 
            	{
            		b[j] = rand() % 1000;
            	}
                a[i][j] = rand() % 1000;
            }
        }
 
        /* Printing the Matrix*/
 
        printf("Matrix :\n");
        for (int i=0;i<AROW;i++)
        {
            for (int j=0;j<ACOL;j++)
            {
                printf("%3f ", a[i][j]);
            }
            printf("\n");
        }
        printf("\nVector :\n");
        for (int i=0;i<ACOL;i++)
        {
            printf("%3f \n", b[i]);
        }
        printf("\n");
 
        /* (1) Sending B Values to other processes */
        for (int j=1;j<size;j++)
        {
            MPI_Send(b, ACOL, MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
        }
 
        /* (2) Sending Required A Values to specific process */
        for (int i=0;i<AROW;i++)
        {
            int processor = proc_map(i, size);
            MPI_Send(a[i], ACOL, MPI_DOUBLE, processor, (100*(i+1)), MPI_COMM_WORLD);
        }
 
        /* (3) Gathering the result from other processes*/
        printf("Output: \n");
        for (int i=0;i<AROW;i++)
        {
            int source_process = proc_map(i, size);
            MPI_Recv(&c[i], 1, MPI_DOUBLE, source_process, i, MPI_COMM_WORLD, &Stat);
            printf("%f\n",c[i]);
        }
    }
    else
    {
        double b[ACOL];
 
        /* (1) Each process get B Values from Master */
        MPI_Recv(b, ACOL, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &Stat);
 
        /* (2) Get Required A Values from Master then Compute the result */
        for (int i=0;i<AROW;i++)
        {
            int processor = proc_map(i, size);
            if (rank == processor)
            {
                double buffer[ACOL];
                MPI_Recv(buffer, ACOL, MPI_DOUBLE, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                double sum = 0;
                for (int j=0;j<ACOL;j++)
                {
                    sum = sum + (buffer[j] * b[j] );
                }
                MPI_Send(&sum, 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
            }
        }
    }
 
    MPI_Finalize();
    return 0;
}
//mpicc -g3 matMat.c -o matMat
//mpirun -n 4 ./matMat
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define AROW 3
#define ACOL 5
 
#define BCOL 3

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
        int a[AROW][ACOL];
        int b[ACOL][BCOL];
        int c[AROW][BCOL];
 
        /* Generating Random Values for A & B Array*/
        for (int i=0;i<AROW;i++)
        {
            for (int j=0;j<ACOL;j++)
            {
                a[i][j] = rand() % 1000;
            }
        }
 
        for (int i=0;i<ACOL;i++)
        {
            for (int j=0;j<BCOL;j++)
            {
                b[i][j] = rand() % 1000;
            }
        }
 
        /* Printing the Matrix*/
 
        printf("Matrix A :\n");
        for (int i=0;i<AROW;i++)
        {
            for (int j=0;j<ACOL;j++)
            {
                printf("%3d ", a[i][j]);
            }
            printf("\n");
        }
 
        printf("\nMatrix B :\n");
        for (int i=0;i<ACOL;i++)
        {
            for (int j=0;j<BCOL;j++)
            {
                printf("%3d ", b[i][j]);
            }
            printf("\n");
        }
        printf("\n");
 
        /* (1) Sending B Values to other processes */
        for (int j=1;j<size;j++)
        {
            for (int x=0;x<ACOL;x++)
            {
                MPI_Send(b[x], BCOL, MPI_INT, j, 1000 + x, MPI_COMM_WORLD);
            }
        }
 
        /* (2) Sending Required A Values to specific process */
        for (int i=0;i<AROW;i++)
        {
            int processor = proc_map(i, size);
            MPI_Send(a[i], ACOL, MPI_INT, processor, (100*(i+1)), MPI_COMM_WORLD);
        }
 
        /* (3) Gathering the result from other processes*/
        for (int i=0;i<AROW;i++)
        {
            int source_process = proc_map(i, size);
            MPI_Recv(c[i], BCOL, MPI_INT, source_process, i, MPI_COMM_WORLD, &Stat);
        }
 
        /* Printing the Result */
        printf("\nMatrix C :\n");
        for (int i=0;i<AROW;i++)
        {
            for (int x=0;x<BCOL;x++)
            {
                printf("%3d ", c[i][x]);
            }
            printf("\n");
        }
 
    }
    else
    {
        int b[ACOL][BCOL];
 
        /* (1) Each process get B Values from Master */
        for (int x=0;x<ACOL;x++)
        {
            MPI_Recv(b[x], BCOL, MPI_INT, 0, 1000 + x, MPI_COMM_WORLD, &Stat);
        }
 
        /* (2) Get Required A Values from Master then Compute the result */
        for (int i=0;i<AROW;i++)
        {
            int c[BCOL];
            int processor = proc_map(i, size);
            if (rank == processor)
            {
                int buffer[ACOL];
                MPI_Recv(buffer, ACOL, MPI_INT, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                for (int j=0;j<BCOL;j++)
                {
                    int sum = 0;
                    for (int z=0;z<ACOL;z++)
                    {
                        sum = sum + (buffer[z] * b[z][j] );
                    }
                    c[j] = sum;
                }
                MPI_Send(c, BCOL, MPI_INT, 0, i, MPI_COMM_WORLD);
            }
        }
    }
 
    MPI_Finalize();
    return 0;
}
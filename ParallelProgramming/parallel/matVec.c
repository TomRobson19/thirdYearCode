//mpicc -g3 matVec.c -o matVec
//mpirun -n 4 ./matVec
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROW 3
#define COLUMN 5

/* Process mapping function */
int mapProcesses(int i, int size)
{
    size = size - 1;
    int r = ROW/size;
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
        double a[ROW][COLUMN];
        double b[COLUMN];
        double c[ROW];
 
        /* Generating Random Values for A & B Array*/
        for (int i=0;i<ROW;i++)
        {
            for (int j=0;j<COLUMN;j++)
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
        for (int i=0;i<ROW;i++)
        {
            for (int j=0;j<COLUMN;j++)
            {
                printf("%3f ", a[i][j]);
            }
            printf("\n");
        }
        printf("\nVector :\n");
        for (int i=0;i<COLUMN;i++)
        {
            printf("%3f \n", b[i]);
        }
        printf("\n");
 
        /* (1) Sending B Values to other processes */
        for (int j=1;j<size;j++)
        {
            MPI_Send(b, COLUMN, MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
        }
 
        /* (2) Sending Required A Values to specific process */
        for (int i=0;i<ROW;i++)
        {
            int processor = mapProcesses(i, size);
            MPI_Send(a[i], COLUMN, MPI_DOUBLE, processor, (100*(i+1)), MPI_COMM_WORLD);
        }
 
        /* (3) Gathering the result from other processes*/
        printf("Output: \n");
        for (int i=0;i<ROW;i++)
        {
            int source_process = mapProcesses(i, size);
            MPI_Recv(&c[i], 1, MPI_DOUBLE, source_process, i, MPI_COMM_WORLD, &Stat);
            printf("%f\n",c[i]);
        }
    }
    else
    {
        double b[COLUMN];
 
        /* (1) Each process get B Values from Master */
        MPI_Recv(b, COLUMN, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &Stat);
 
        /* (2) Get Required A Values from Master then Compute the result */
        for (int i=0;i<ROW;i++)
        {
            int processor = mapProcesses(i, size);
            if (rank == processor)
            {
                double buffer[COLUMN];
                MPI_Recv(buffer, COLUMN, MPI_DOUBLE, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                double sum = 0;
                for (int j=0;j<COLUMN;j++)
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
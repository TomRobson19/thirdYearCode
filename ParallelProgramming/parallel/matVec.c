//mpicc -g3 matVec.c -o matVec
//mpirun -n 4 ./matVec
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/* Process mapping function */
int mapProcesses(int i, int size, int row)
{
    size = size - 1;
    int r = row/size;
    int proc = i / r;
    return proc;
}

int main(int argc, char** argv)
{
	srand(0);

	int row = atoi(argv[1]);
	int column = atoi(argv[2]);

    int size, rank;
    MPI_Status Stat;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    if (rank == 0)
    {
        double a[row][column];
        double b[column];
        double c[row];
        /* Generating Random Values for A & B Array*/
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (i==0) 
            	{
            		b[j] = rand() % 10;
            	}
                a[i][j] = rand() % 10;
            }
        }
        /* Printing the Matrix*/
        printf("Matrix :\n");
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                printf("%3f ", a[i][j]);
            }
            printf("\n");
        }
        printf("\nVector :\n");
        for (int i=0;i<column;i++)
        {
            printf("%3f \n", b[i]);
        }
        printf("\n");
 
        /* (1) Sending B Values to other processes */
        for (int j=1;j<size;j++)
        {
            MPI_Send(b, column, MPI_DOUBLE, j, 99, MPI_COMM_WORLD);
        }
 
        /* (2) Sending Required A Values to specific process */
        for (int i=1;i<row;i++)
        {
            int processor = mapProcesses(i, size, row);
            MPI_Send(a[i], column, MPI_DOUBLE, processor, (100*(i+1)), MPI_COMM_WORLD);
        }

        for (int j=0;j<column;j++)
        {
            c[0] = c[0] + (a[0][j] * b[j] );
        }
 
        /* (3) Gathering the result from other processes*/
        printf("Output: \n");
        for (int i=1;i<row;i++)
        {
            int source_process = mapProcesses(i, size, row);
            MPI_Recv(&c[i], 1, MPI_DOUBLE, source_process, i, MPI_COMM_WORLD, &Stat);
        }
        for (int i=0;i<row;i++)
        {
        	printf("%f\n",c[i]);
        }
    }
    else
    {
        double b[column];
 
        /* (1) Each process get B Values from Master */
        MPI_Recv(b, column, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &Stat);
 
        /* (2) Get Required A Values from Master then Compute the result */
        for (int i=1;i<row;i++)
        {
            int processor = mapProcesses(i, size, row);
            if (rank == processor)
            {
                double buffer[column];
                MPI_Recv(buffer, column, MPI_DOUBLE, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                double sum = 0;
                for (int j=0;j<column;j++)
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
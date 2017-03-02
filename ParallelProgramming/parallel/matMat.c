//mpicc -g3 matMat.c -o matMat
//mpirun -n 4 ./matMat
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/* Process mapping function */
int mapProcesses(int i, int size, int arow)
{
    size = size - 1;
    int r = arow/size;
    int proc = i / r;
    return proc + 1;
}

int main(int argc, char** argv)
{
	srand(0);

	int arow = atoi(argv[1]);
	int acolumn = atoi(argv[2]);
	int bcolumn = atoi(argv[3]);

    int size, rank;
    MPI_Status Stat;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    if (rank == 0)
    {
        double a[arow][acolumn];
        double b[acolumn][bcolumn];
        double c[arow][bcolumn];
        /* Generating Random Values for A & B Array*/
        for (int i=0;i<arow;i++)
        {
            for (int j=0;j<acolumn;j++)
            {
                a[i][j] = rand() % 1000;
            }
        }
        for (int i=0;i<acolumn;i++)
        {
            for (int j=0;j<bcolumn;j++)
            {
                b[i][j] = rand() % 1000;
            }
        }
        /* Printing the Matrix*/
        printf("Matrix A :\n");
        for (int i=0;i<arow;i++)
        {
            for (int j=0;j<acolumn;j++)
            {
                printf("%3f ", a[i][j]);
            }
            printf("\n");
        }
        printf("\nMatrix B :\n");
        for (int i=0;i<acolumn;i++)
        {
            for (int j=0;j<bcolumn;j++)
            {
                printf("%3f ", b[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        /* (1) Sending B Values to other processes */
        for (int j=1;j<size;j++)
        {
            for (int x=0;x<acolumn;x++)
            {
                MPI_Send(b[x], bcolumn, MPI_DOUBLE, j, 1000 + x, MPI_COMM_WORLD);
            }
        }
        /* (2) Sending Required A Values to specific process */
        for (int i=0;i<arow;i++)
        {
            int processor = mapProcesses(i, size, arow);
            MPI_Send(a[i], acolumn, MPI_DOUBLE, processor, (100*(i+1)), MPI_COMM_WORLD);
        }
 
        /* (3) Gathering the result from other processes*/
        for (int i=0;i<arow;i++)
        {
            int source_process = mapProcesses(i, size, arow);
            MPI_Recv(c[i], bcolumn, MPI_DOUBLE, source_process, i, MPI_COMM_WORLD, &Stat);
        }
        /* Printing the Result */
        printf("\nMatrix C :\n");
        for (int i=0;i<arow;i++)
        {
            for (int x=0;x<bcolumn;x++)
            {
                printf("%3f ", c[i][x]);
            }
            printf("\n");
        }
    }
    else
    {
        double b[acolumn][bcolumn];
        /* (1) Each process get B Values from Master */
        for (int x=0;x<acolumn;x++)
        {
            MPI_Recv(b[x], bcolumn, MPI_DOUBLE, 0, 1000 + x, MPI_COMM_WORLD, &Stat);
        }
        /* (2) Get Required A Values from Master then Compute the result */
        for (int i=0;i<arow;i++)
        {
            double c[bcolumn];
            int processor = mapProcesses(i, size, arow);
            if (rank == processor)
            {
                double buffer[acolumn];
                MPI_Recv(buffer, acolumn, MPI_DOUBLE, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                for (int j=0;j<bcolumn;j++)
                {
                    double sum = 0;
                    for (int z=0;z<acolumn;z++)
                    {
                        sum = sum + (buffer[z] * b[z][j] );
                    }
                    c[j] = sum;
                }
                MPI_Send(c, bcolumn, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Finalize();
    return 0;
}
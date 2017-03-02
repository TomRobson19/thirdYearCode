//mpicc -g3 matMat.c -o matMat
//mpirun -n 4 ./matMat
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	srand(0);
    if (argc < 2) {
        printf("Syntax: test <size of vector>\n");
        return 1;
    }

    double totalTime = 0;

	int arow = atoi(argv[1]);
	int acolumn = atoi(argv[2]);
	int bcolumn = atoi(argv[3]);

	double * matrix1;
	double * matrix2;
	double * output;

	double * matrixTemporary;
	double * vectorTemporary;
	double * outputTemporary;

	int totalNumberOfProcesses;

	int rank;
	
	int i;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &totalNumberOfProcesses);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// int rowsPerProcess = (int) (ceil( ( (double) row)/( (double) totalNumberOfProcesses)));

	// int totalRows = rowsPerProcess*totalNumberOfProcesses;

	matrix1 = (double *) calloc(arow*acolumn,sizeof(double));
	matrix2 = (double *) calloc(acolumn*bcolumn,sizeof(double));
	output = (double *) calloc(arow*bcolumn,sizeof(double));

	if (rank == 0)
	{
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
	}

}
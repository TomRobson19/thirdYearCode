//mpicc -g3 matMat.c -o matMat
//mpirun -lm -n 4 ./matMat
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	srand(0);
    if (argc < 2) {
        printf("Syntax: test <size of vector>\n");
        return 1;
    }

    double totalTime = 0;

	int arow = atoi(argv[1]);
	int acolumnbrow = atoi(argv[2]);
	int bcolumn = atoi(argv[3]);

	double * matrix1;
	double * matrix2;
	double * output;

	double * matrix1Temporary;
	double * matrix2Temporary;
	double * outputTemporary;

	int totalNumberOfProcesses;

	int rank;
	
	int i;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &totalNumberOfProcesses);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int rowsPerProcess = (int) (ceil( ( (double) acolumnbrow)/( (double) totalNumberOfProcesses)));

	int totalRows = rowsPerProcess*totalNumberOfProcesses;

	matrix1 = (double *) calloc(arow*acolumnbrow,sizeof(double));
	matrix2 = (double *) calloc(acolumnbrow*bcolumn,sizeof(double));
	output = (double *) calloc(arow*bcolumn,sizeof(double));

	if (rank == 0)
	{
		for (int i=0;i<arow*totalRows;i++)
        {
            matrix1[i] = rand() % 1000;
        }
        for (int i=0;i<totalRows*bcolumn;i++)
        {
            matrix2[i] = rand() % 1000;
        }
        /* Printing the Matrix*/
        printf("Matrix A :\n");
        for (int i=0;i<arow*acolumnbrow;i++)
        {
            if(i%acolumnbrow == 0)
            {
                printf("\n");
            }
            printf("%3f ", matrix1[i]);
        }
        printf("\n");
        printf("\nMatrix B :\n");
        for (int i=0;i<acolumnbrow*bcolumn;i++)
        {
            if(i%bcolumn == 0)
            {
                printf("\n");
            }
            printf("%3f ", matrix2[i]);
        }
        printf("\n");
	}

	matrix1Temporary = (double *) calloc(arow*acolumnbrow,sizeof(double));
	matrix2Temporary = (double *) calloc(acolumnbrow*bcolumn,sizeof(double));
	outputTemporary = (double *) calloc(arow*bcolumn,sizeof(double));

	MPI_Bcast(matrix1, arow*totalRows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatter(matrix2, acolumnbrow*rowsPerProcess, MPI_DOUBLE, matrix2Temporary, acolumnbrow*rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double startTime = MPI_Wtime();

	///MATHS GOES HERE





	double endTime = MPI_Wtime();

	//MPI_Gather(outputTemporary, rowsPerProcess, MPI_DOUBLE, output, rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double temporaryTime = endTime - startTime;

	//MPI_Reduce(&temporaryTime, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		printf("\nOutput :\n");
        for (int i=0;i<arow*bcolumn;i++)
        {
            if(i%bcolumn == 0)
            {
                printf("\n");
            }
            printf("%3f ", output[i]);
        }
        printf("\n");

		printf("\nTime\n");
		printf("%f\n", totalTime);
	}

	MPI_Finalize();
	return 0;
}


////row of A x column of B

////broadcast A to all processes, send out B row by row
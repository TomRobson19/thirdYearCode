//mpicc -g3 matVec.c -o matVec
//mpirun -lm -n 4 ./matVec
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

	int row = atoi(argv[1]);
	int column = atoi(argv[2]);

	double * matrix;
	double * vector;
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

	int rowsPerProcess = (int) (ceil( ( (double) row)/( (double) totalNumberOfProcesses)));

	int totalRows = rowsPerProcess*totalNumberOfProcesses;

	vector = (double *) calloc(column,sizeof(double));
	matrix = (double *) calloc(column*totalRows,sizeof(double));
	output = (double *) calloc(totalRows,sizeof(double));

	if (rank == 0)
	{	
		for (int i=0;i<row*column;i++)
        {
            if (i<column) 
        	{
        		vector[i] = rand() % 1000;
        	}
            matrix[i] = rand() % 1000;
        }
		printf("Matrix :");
        for (int i=0;i<row*column;i++)
        {
            if(i%column == 0)
            {
                printf("\n");
            }
            printf("%3f ", matrix[i]);
        }
        printf("\n\nVector :\n");
        for (int i=0;i<column;i++)
        {
            printf("%3f \n", vector[i]);
        }
    }

	matrixTemporary = (double *) calloc(column*rowsPerProcess,sizeof(double));
	outputTemporary = (double *) calloc(totalRows,sizeof(double));
	
	MPI_Bcast(vector, column, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatter(matrix, column*rowsPerProcess, MPI_DOUBLE, matrixTemporary, column*rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double startTime = MPI_Wtime();

	for(int i=0;i<rowsPerProcess;i++)
	{
		for (int j=0;j<column;j++)
	    {
	        outputTemporary[i] += (matrixTemporary[(i*column)+j] * vector[j]);
	    }
	}

	double endTime = MPI_Wtime();

	MPI_Gather(outputTemporary, rowsPerProcess, MPI_DOUBLE, output, rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double temporaryTime = endTime - startTime;

	MPI_Reduce(&temporaryTime, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		printf("\nOutput :\n");
        for (int i=0;i<row;i++)
        {	
            printf("%3f \n", output[i]);
        }

		printf("\nTime\n");
		printf("%f\n", totalTime);
	}

	MPI_Finalize();
	return 0;
}
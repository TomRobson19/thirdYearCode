//mpicc -g3 vecDot.c -o vecDot
//mpirun -n 4 ./vecDot
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

	int elementsAllocatedPerProcess;
	
	int i;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &totalNumberOfProcesses);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	vector = (double *) calloc(column,sizeof(double));
	matrix = (double *) calloc(column*row,sizeof(double));
	output = (double *) calloc(row,sizeof(double));

	if (rank == 0)
	{	
		for (int i=0;i<row*column;i++)
        {
            if (i<column) 
        	{
        		vector[i] = rand() % 10;
        	}
            matrix[i] = rand() % 10;
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

	matrixTemporary = (double *) calloc(column,sizeof(double));
	outputTemporary = (double *) calloc(1,sizeof(double));
	
	MPI_Bcast(vector, column, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatter(matrix, column, MPI_DOUBLE, matrixTemporary, column, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double startTime = MPI_Wtime();

	for (int j=0;j<column;j++)
    {
        outputTemporary[0] += (matrixTemporary[j] * vector[j]);
    }

	double endTime = MPI_Wtime();

	MPI_Gather(outputTemporary, 1, MPI_DOUBLE, output, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double temporaryTime = endTime - startTime;

	MPI_Reduce(&temporaryTime, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		printf("\nOutput :\n");
        for (int i=0;i<row;i++)
        {
            printf("%3f \n", output[i]);
        }

		printf("Time\n");
		printf("%f\n", totalTime);
	}

	MPI_Finalize();
	return 0;
}
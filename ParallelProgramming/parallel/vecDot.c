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
    int vectorSize = atoi(argv[1]);
	double * vector1;
	double * vector2;
	double dotProduct = 0;

	int totalNumberOfProcesses;

	int rank;

	int elementsAllocatedPerProcess;
	
	int i;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &totalNumberOfProcesses);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	double * vector1Temporary;
	double * vector2Temporary;
	double sumTemporary = 0;
	if (rank == 0)
	{
		vector1 = (double *) malloc(sizeof(double)*vectorSize);
		vector2 = (double *) malloc(sizeof(double)*vectorSize);

    	MPI_Bcast (&vectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    	elementsAllocatedPerProcess = vectorSize/totalNumberOfProcesses;

		for(i=0;i<vectorSize;i++)
		{
			vector1[i] = rand() % 1000;
			vector2[i] = rand() % 1000;
		}
		printf("Vector1: \n");
		for(i=0;i<vectorSize;i++)
		{
			printf ("%f\n", vector1[i]);
		}	
		printf("Vector2: \n");
		for(i=0;i<vectorSize;i++)
		{
			printf ("%f\n", vector2[i]);
		}	
		if(vectorSize%totalNumberOfProcesses != 0)
		{
	    	elementsAllocatedPerProcess+=1;
	    	for(i=0;i<(elementsAllocatedPerProcess*totalNumberOfProcesses - vectorSize);i++)
	    	{
	    		vector1[vectorSize+i] = 0;
	    		vector2[vectorSize+i] = 0;
	    	}
		}
		vector1Temporary = (double *) malloc(sizeof(double)*elementsAllocatedPerProcess);
		vector2Temporary = (double *) malloc(sizeof(double)*elementsAllocatedPerProcess);

		MPI_Bcast(&elementsAllocatedPerProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);

		MPI_Scatter(vector1, elementsAllocatedPerProcess, MPI_DOUBLE, vector1Temporary, elementsAllocatedPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		MPI_Scatter(vector2, elementsAllocatedPerProcess, MPI_DOUBLE, vector2Temporary, elementsAllocatedPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		for(i=0;i<elementsAllocatedPerProcess;i++)
		{
			sumTemporary = vector1Temporary[i]*vector2Temporary[i];
		}

		MPI_Reduce(&sumTemporary, &dotProduct, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		printf("Dot Product\n");
		printf("%f\n", dotProduct);
    }
	else
	{
		MPI_Bcast(&vectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&elementsAllocatedPerProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		vector1Temporary = (double *) malloc(sizeof(double)*elementsAllocatedPerProcess);
		vector2Temporary = (double *) malloc(sizeof(double)*elementsAllocatedPerProcess);
		
		MPI_Scatter(vector1, elementsAllocatedPerProcess, MPI_DOUBLE, vector1Temporary, elementsAllocatedPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatter(vector2, elementsAllocatedPerProcess, MPI_DOUBLE, vector2Temporary, elementsAllocatedPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		for(i=0;i<elementsAllocatedPerProcess;i++)
		{
			sumTemporary = vector1Temporary[i]*vector2Temporary[i];
		}
		MPI_Reduce(&sumTemporary, &dotProduct, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
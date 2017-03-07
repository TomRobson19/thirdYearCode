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
        //printf("Syntax: test <size of vector>\n");
        return 1;
    }

    double totalTime = 0;

	int arow = atoi(argv[1]);
	int acolumnbrow = atoi(argv[2]);
	int bcolumn = arow;

	double * matrix1;
	double * matrix2;
	double * output;

	double * matrix1Temporary;
	double * matrix2Temporary;
	double * outputTemporary;

	int totalNumberOfProcesses;

	int rank;
	
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &totalNumberOfProcesses);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int rowsPerProcess = (int) (ceil( ( (double) arow)/( (double) totalNumberOfProcesses)));

	int totalRows = rowsPerProcess*totalNumberOfProcesses;

	matrix1 = (double *) calloc(totalRows*acolumnbrow,sizeof(double));
	matrix2 = (double *) calloc(acolumnbrow*bcolumn,sizeof(double));
	output = (double *) calloc(totalRows*bcolumn,sizeof(double));

	if (rank == 0)
	{
		for (int i=0;i<arow*acolumnbrow;i++)
        {
            matrix1[i] = rand() % 1000;
        }
        for (int i=0;i<acolumnbrow*bcolumn;i++)
        {
            matrix2[i] = rand() % 1000;
        }
        /* Printing the Matrix*/
        // printf("Matrix A :\n");
        // for (int i=0;i<arow*acolumnbrow;i++)
        // {
        //     if(i%acolumnbrow == 0)
        //     {
        //         printf("\n");
        //     }
        //     printf("%3f ", matrix1[i]);
        // }
        // printf("\n");
        // printf("\nMatrix B :\n");
        // for (int i=0;i<acolumnbrow*bcolumn;i++)
        // {
        //     if(i%bcolumn == 0)
        //     {
        //         printf("\n");
        //     }
        //     printf("%3f ", matrix2[i]);
        // }
        // printf("\n");
	}

	matrix1Temporary = (double *) calloc(totalRows*acolumnbrow,sizeof(double));
	outputTemporary = (double *) calloc(totalRows*bcolumn,sizeof(double));

	MPI_Bcast(matrix2, acolumnbrow*bcolumn, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatter(matrix1,acolumnbrow*rowsPerProcess,MPI_DOUBLE,matrix1Temporary,acolumnbrow*rowsPerProcess,MPI_DOUBLE,0,MPI_COMM_WORLD);

	double startTime = MPI_Wtime();

	for(int i=0;i<rowsPerProcess;i++)
	{
		for(int j=0;j<bcolumn;j++)
	    {
	    	double dotProduct = 0;
	    	for(int k=0;k<acolumnbrow;k++)
	    	{
	    		dotProduct += matrix1Temporary[(i*acolumnbrow)+k] * matrix2[(k*bcolumn)+j];
	    	}
	    	outputTemporary[(i*bcolumn)+j] = dotProduct;
	    }
	}

	double endTime = MPI_Wtime();

	MPI_Gather(outputTemporary, rowsPerProcess*bcolumn, MPI_DOUBLE, output, rowsPerProcess*bcolumn, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double temporaryTime = endTime - startTime;

	MPI_Reduce(&temporaryTime, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		// printf("\nOutput :\n");
  //       for (int i=0;i<arow*bcolumn;i++)
  //       {
  //           if(i%bcolumn == 0)
  //           {
  //               printf("\n");
  //           }
  //           printf("%3f ", output[i]);
  //       }
  //       printf("\n");

		printf("Time matMat size %d %d\n",arow,acolumnbrow);
		printf("%f\n", totalTime);
	}

	MPI_Finalize();
	return 0;
}

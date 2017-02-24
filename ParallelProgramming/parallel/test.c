#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "linearAlgebra.h"

int output_vector(double* vector, int length);
int output_matrix(double** matrix, int size1, int size2);

int main(int argc, char* argv[]) {
    srand(0);
    if (argc < 2) {
        printf("Syntax: test <size of vector>\n");
        return 1;
    }
    int v_size = 0;
    int v_size2 = 0;
    if (argc < 3) {
        v_size = atoi(argv[1]);
        v_size2 = v_size;
    } else {
        v_size = atoi(argv[1]);
        v_size2 = atoi(argv[2]);
    }
    
    printf("Creating two vectors of size %d\n", v_size);
    double* vector1 = (double*) malloc(sizeof(double) * v_size);
    double* vector2 = (double*) malloc(sizeof(double) * v_size);
    
    for (int i = 0; i < v_size; i++) {
        vector1[i] = rand() % 1000;
        vector2[i] = rand() % 1000;
    }
    
    double** matrix1 = (double**) malloc(sizeof(double*) * v_size);
    double** matrix2 = (double**) malloc(sizeof(double*) * v_size2);
    
    for (int i = 0; i < v_size; i++) {
        matrix1[i] = (double*) malloc(sizeof(double) * v_size2);
        for (int j = 0; j < v_size2; j++) {
            matrix1[i][j] = rand() % 1000;
        }
    }

    for (int i = 0; i < v_size2; i++) {
        matrix2[i] = (double*) malloc(sizeof(double) * v_size);
        for (int j = 0; j < v_size; j++) {
            matrix2[i][j] = rand() % 1000;
        }
    }
    
    printf("Vectors are : \n");
    
    output_vector(vector1, v_size);
    output_vector(vector2, v_size);
    
    printf("Vector addition is : \n");
    
    output_vector(vector_vector_addition(vector1, vector2, v_size), v_size);
    
    printf("Vector dot product is : %f \n", vector_vector_dot_prod(vector1, vector2, v_size));
    
    printf("Matrix1 is : \n");
    output_matrix(matrix1, v_size, v_size2);
    
    printf("Matrix2 is : \n");
    output_matrix(matrix2, v_size2, v_size);
    
    printf("Matrix2 * vector1 is : \n");
    
    output_vector(matrix_vector_multiplication(matrix2, vector1, v_size2, v_size), v_size2);
    
    printf("Matrix matrix multiplication is : \n");
    
    output_matrix(matrix_matrix_multiplication(matrix1, matrix2, v_size, v_size2, v_size), v_size, v_size);
}

int output_vector(double* vector, int length) {
    printf("[ ");
    for (int i = 0; i < length; i++) {
        if (i < length - 1)
            printf(" %.0f ;", vector[i]);
        else
            printf(" %.0f ", vector[i]);
    }
    printf(" ]\n");
    return 1;
}

int output_matrix(double** matrix, int rows, int columns) {
    printf("[ ");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf(" %.0f ", matrix[i][j]);
        }
        if (i < rows - 1)
            printf(";\n");
    }
    printf(" ]\n");
    return 1;
}

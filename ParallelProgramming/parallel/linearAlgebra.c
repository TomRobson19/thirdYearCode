#include "linearAlgebra.h"

double* vector_vector_addition(double* a, double *b, int length) {
    // in matrix form: result = a + b
    double* result = (double*) malloc(sizeof(double) * length);
    for (int index = 0; index < length; index++)
        result[index] = a[index] + b[index];
    return result;
}

double vector_vector_dot_prod(double *a, double *b, int length) {
    // return the dot product of two vectors
    double runningSum = 0.0;
    for (int index = 0; index < length; index++)
        runningSum += a[index] * b[index];
    return runningSum;
}

double* matrix_vector_multiplication(double **mat, double *vec, int rows, int cols) {
    // in matrix form: result = mat * vec;
    double* result = (double*) malloc(sizeof(double) * rows);
    int i;
    for (i = 0; i < rows; i++) {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
            result[i] += mat[i][j] * vec[j];
    }
    return result;
}

double** matrix_matrix_multiplication(double **a, double **b, int rows_a, int columns_a, int columns_b) {
    // in matrix form: result = a * b
    double ** mult = (double**) malloc(sizeof(double*) * rows_a);
    for(int i = 0; i < rows_a; ++i) {
        mult[i] = (double*) malloc(sizeof(double) * columns_b);
        for(int j = 0; j < columns_b; ++j) {
            mult[i][j] = 0.0;
            for(int k = 0; k < columns_a; ++k) {
                mult[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
    return mult;
}
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> // inkludiere den MPI-Header


// ---------------------------------------------------------------------------
// allocate space for empty matrix A[row][col]
// access to matrix elements possible with:
// - A[row][col]
// - A[0][row*col]

float **alloc_mat(int row, int col)
{
    float **A1, *A2;

  A1 = (float **)calloc(row, sizeof(float *));    // pointer on rows
  A2 = (float *)calloc(row*col, sizeof(float));    // all matrix elements
    for (int i = 0; i < row; i++)
        A1[i] = A2 + i*col;

    return A1;
}

// ---------------------------------------------------------------------------
// random initialisation of matrix with values [0..9]

void init_mat(float **A, int row, int col)
{
    for (int i = 0; i < row*col; i++)
    A[0][i] = (float)(rand() % 10);
}

// ---------------------------------------------------------------------------
// DEBUG FUNCTION: printout of all matrix elements

void print_mat(float **A, int row, int col, char *tag)
{
    int i, j;

    printf("Matrix %s:\n", tag);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
            printf("%6.1f   ", A[i][j]);
        printf("\n");
    }
}

// ---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  float **A, **B, **C;  // matrices
    int d1 = 1000, d2 = 1000, d3 = 1000;         // dimensions of matrices
    int i, j, k;      // loop variables

    /* prepare matrices */
    A = alloc_mat(d1, d2);
    init_mat(A, d1, d2);
    B = alloc_mat(d2, d3);
    init_mat(B, d2, d3);

    C = alloc_mat(d1, d3);  // no initialisation of C, because it gets filled by matmult

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* serial version of matmult */
    printf("Perform matrix multiplication...\n");
    if (rank == 0) {
      for (i = 0; i < d1; i++) {
          MPI_Send(A, 1000, MPI_FLOAT, i+1, i, MPI_COMM_WORLD);
      }
    } else {
      // MPI_Recv()
      // for (j = 0; j < d3; j++)
      //   for (k = 0; k < d2; k++)
      //     C[i][j] += A[i][k] * B[k][j];
    }


    /* test output */
    // print_mat(A, d1, d2, "A");
    // print_mat(B, d2, d3, "B");
    // print_mat(C, d1, d3, "C");

    printf ("\nDone.\n");

    return 0;
}

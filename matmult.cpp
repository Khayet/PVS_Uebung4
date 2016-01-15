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

int main_var1(int argc, char *argv[])
{
  float **A, **B, **C;  // matrices
    int d1 = 1000, d2 = 99, d3 = 995;         // dimensions of matrices
    // int i, j, k;      // loop variables

    /* prepare matrices */
    A = alloc_mat(d1, d2);
    init_mat(A, d1, d2);
    B = alloc_mat(d2, d3);
    init_mat(B, d2, d3);

    C = alloc_mat(d1, d3);  // no initialization of C, because it gets filled by matmult


    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status rec_stat, rec_stat2;
    int recv_messages = 0;

    printf("Perform matrix multiplication...\n");

    if (rank == 0) {
      for (int i = 0; i < d1; i++) {
          MPI_Send(*A + i*d2, d2, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD);
          MPI_Recv(*C + i*d3, d3, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD, &rec_stat2);
          printf("Row i=%i \n", i);
      }
      /* test output */
      print_mat(A, d1, d2, "A");
      print_mat(B, d2, d3, "B");
      print_mat(C, d1, d3, "C");

    } else {
      int count_ = 0;
      for (int i = 0; i < d1; i++) {
        if (rank == i%(size-1) + 1) {
          ++count_;
        }
      }

      float *rec = (float *)calloc(d2, sizeof(float));
      float *result = (float *)calloc(d3, sizeof(float));

      for (int i = 0; i < count_; i++) {
        MPI_Recv(rec, d2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &rec_stat);

        if (rec_stat.MPI_ERROR == 0) ++recv_messages;
        printf("Rank of process: %i Number of messages received: %i \n", rank, recv_messages);

        for (int j = 0; j < d3; j++) {
          result[j] = 0;
          for (int k = 0; k < d2; k++) {
            result[j] += rec[k] * B[k][j];
          }
        }

        MPI_Send(result, d3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
      }
    }

    MPI_Finalize();


    printf ("\nDone.\n");

    return 0;
}

int main_var2(int argc, char *argv[])
{
  float **A, **B, **C;  // matrices
    int d1 = 100, d2 = 50, d3 = 50;         // dimensions of matrices
    // int i, j, k;      // loop variables

    /* prepare matrices */
    A = alloc_mat(d1, d2);
    init_mat(A, d1, d2);
    B = alloc_mat(d2, d3);
    init_mat(B, d2, d3);

    C = alloc_mat(d1, d3);  // no initialization of C, because it gets filled by matmult


    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status rec_stat, rec_stat2;
    int recv_messages = 0;

    // printf("Perform matrix multiplication...\n");

    MPI_Request req;

    if (rank == 0) {
      for (int i = 0; i < d1; i++) {
          MPI_Isend(*A + i*d2, d2, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD, &req);
          MPI_Recv(*C + i*d3, d3, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD, &rec_stat);
          // printf("Row i=%i \n", i);
      }
      /* test output */
      // print_mat(A, d1, d2, "A");
      // print_mat(B, d2, d3, "B");
      // print_mat(C, d1, d3, "C");

    } else {
      int count_ = 0;
      for (int i = 0; i < d1; i++) {
        if (rank == i%(size-1) + 1) {
          ++count_;
        }
      }

      float *rec = (float *)calloc(d2, sizeof(float));
      float *result = (float *)calloc(d3, sizeof(float));

      for (int i = 0; i < count_; i++) {
        MPI_Recv(rec, d2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &rec_stat);

        if (rec_stat.MPI_ERROR == 0) ++recv_messages;
        // printf("Rank of process: %i Number of messages received: %i \n", rank, recv_messages);

        for (int j = 0; j < d3; j++) {
          result[j] = 0;
          for (int k = 0; k < d2; k++) {
            result[j] += rec[k] * B[k][j];
          }
        }

        MPI_Isend(result, d3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req);
      }
    }

    MPI_Finalize();


    printf ("\nDone.\n");

    return 0;
}

int main_var3(int argc, char *argv[])
{
  float **A, **B, **C;  // matrices
    int d1 = 1000, d2 = 99, d3 = 995;         // dimensions of matrices
    // int i, j, k;      // loop variables

    // d1 := rows of A and C
    // d2 := cols of A and rows of B
    // d3 := cols of B and C

    /* prepare matrices */
    A = alloc_mat(d1, d2);
    B = alloc_mat(d2, d3);
    C = alloc_mat(d1, d3);  // no initialization of C, because it gets filled by matmult

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status rec_stat, rec_stat2;
    int recv_messages = 0;

    // printf("Perform matrix multiplication...\n");
    if (rank == 0) { //A and B only have to be initialized by the master
      init_mat(A, d1, d2);
      init_mat(B, d2, d3);
    }
    MPI_Bcast(*B, d2*d3, MPI_FLOAT, 0, MPI_COMM_WORLD); //Broadcast B to all

    if (rank == 0) {
      for (int i = 0; i < d1; i++) {
          MPI_Send(*A + i*d2, d2, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD);
          MPI_Recv(*C + i*d3, d3, MPI_FLOAT, i%(size-1) + 1, 0, MPI_COMM_WORLD, &rec_stat2);
          // printf("Row i=%i \n", i);
      }
      /* test output */
      // print_mat(A, d1, d2, "A");
      // print_mat(B, d2, d3, "B");
      // print_mat(C, d1, d3, "C");

    } else {
      int count_ = 0;
      for (int i = 0; i < d1; i++) {
        if (rank == i%(size-1) + 1) {
          ++count_;
        }
      }

      float *rec = (float *)calloc(d2, sizeof(float));
      float *result = (float *)calloc(d3, sizeof(float));

      for (int i = 0; i < count_; i++) {
        MPI_Recv(rec, d2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &rec_stat);

        if (rec_stat.MPI_ERROR == 0) ++recv_messages;
        // printf("Rank of process: %i Number of messages received: %i \n", rank, recv_messages);

        for (int j = 0; j < d3; j++) {
          result[j] = 0;
          for (int k = 0; k < d2; k++) {
            result[j] += rec[k] * B[k][j];
          }
        }

        MPI_Send(result, d3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
      }
    }

    MPI_Finalize();


    printf ("\nDone.\n");

    return 0;
}


int main(int argc, char *argv[])
{
  // main_var1(argc, argv);
  // main_var2(argc, argv);
  main_var3(argc, argv);
  return 0;
}

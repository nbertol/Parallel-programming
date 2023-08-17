//how to transport a 3x3 matrix

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4

int main(int argc, char** argv) {
int rank=0, size=0;
int i, j;
int A[N][N], B[N][N];
char processor_name[MPI_MAX_PROCESSOR_NAME];
int name_len;
int world_rank;
int world_size;


MPI_Status status;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

if (rank == 0) {
// Initialize matrix A
for (i = 0; i < N; i++) {
for (j = 0; j < N; j++) {
A[i][j] = i * N + j;
}
}

// Print matrix A
printf("Matrix A\n");
for (i = 0; i < N; i++) {
for (j = 0; j < N; j++) {
printf("%d ", A[i][j]);
}
printf("\n");
}

// Send matrix A to all processes
for (i = 1; i < size; i++) {
MPI_Send(A, N * N, MPI_INT, i, 0, MPI_COMM_WORLD);
// printf("%d send\n", size);
}
}
else {
// Receive matrix A from process 0
MPI_Recv(A, N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
// printf("recieve\n");
}

// Transpose matrix A
for (i = 0; i < N; i++) {
for (j = 0; j < N; j++) {
B[j][i] = A[i][j];
}
}

if (rank == 0) {
// Receive transposed matrix B from all processes
for (i = 1; i < size; i++) {
MPI_Recv(B, N * N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

// Merge transposed matrix B into matrix A
for (j = 0; j < N; j++) {
for (int k = 0; k < N; k++) {
A[j][k] = B[j][k];
}
}

}

// Print matrix A
printf("Transposed matrix A\n");
for (i = 0; i < N; i++) {
for (j = 0; j < N; j++) {
printf("%d ", A[i][j]);
}
printf("\n");
}
}
else {
// Send transposed matrix B to process 0
MPI_Send(B, N * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
MPI_Get_processor_name(processor_name, &name_len);

MPI_Comm_size(MPI_COMM_WORLD, &world_size);
printf("Hello from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);
MPI_Finalize();
return 0;
}
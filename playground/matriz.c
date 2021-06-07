#define N 6

typedef int fix_matrix[N][N];

int fix_prod_ele (fix_matrix A, fix_matrix B, int i , int k) {
  int j;
  int result = 0;

  for (j=0;j<N;j++)
    result += A[i][j] * B[j][k];

  return result;
}
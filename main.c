#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

long double factorial(int x);

int main(int argc, char **argv)
{
  uint64_t upper_limit;
  int32_t rank,
      size,
      block_size,
      angle_deg,
      start = 0,
      *block;
  long double result = 0.0, num, den;

  MPI_Status status;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
  {
    printf("please enter the upper limit of k : ");
    scanf("%d", &upper_limit);
    printf("please enter the angle x in degree : ");
    scanf("%d", &angle_deg);

    printf("upper limit is %d,the angle is %d and no. of processes are %d\n", upper_limit, angle_deg, size);

    block_size = upper_limit / size;
    block = malloc(block_size * sizeof(int));
    int new_start = start + block_size;

    for (int i = 1; i < size; i++)
    {
      MPI_Send(&block_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&new_start, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      MPI_Send(&angle_deg, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
      new_start += block_size;
    }
  }
  else
  {
    MPI_Recv(&block_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&angle_deg, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    block = malloc(block_size * sizeof(int));
  }

  float start_time = MPI_Wtime();

  for (size_t i = 0; i < block_size; i++)
  {
    float angle_rad = angle_deg / 180.0;
    long long unsigned int value = start + i;
    num = pow(-1, value);
    num *= pow(M_PI * angle_rad, 2 * value);
    den = factorial(2 * value);
    num /= den;
    result += num;
  }

  if (rank != 0)
  {
    MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else
  {
    for (int i = 1; i < size; i++)
    {
      long double ans = 0.0;
      MPI_Recv(&ans, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
      result += ans;
    }

    printf("The result: %Lf\n", result);
    printf("The total time for calculation of cosine is %f\n", MPI_Wtime() - start_time);
  }

  MPI_Finalize();

  return 0;
}

long double factorial(int num)
{
  long double fact = 1;
  for (int i = 1; i <= num; i++)
  {
    fact *= i;
  }
  return fact;
}
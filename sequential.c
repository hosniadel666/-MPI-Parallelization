#include <stdio.h>
#include <math.h>
#include <time.h>

long double fact(int x)
{
  long double result = 1;
  for (int i = 1; i <= x; i++)
  {
    result *= i;
  }
  return result;
}

int main(int argc, char **argv)
{
  long unsigned int upper_limit;
  int rank, size, buf_size, angle_deg;
  int start = 0;
  int *buf;
  long double result = 0.0, num, den;
  printf("please enter the upper limit of k : ");
  scanf("%d", &upper_limit);
  printf("please enter the angle x in degree : ");
  scanf("%d", &angle_deg);

  clock_t begin = clock();
  for (size_t i = 0; i < upper_limit; i++)
  {
    float angle_rad = angle_deg / 180.0;
    long long unsigned int value = start + i;
    num = pow(-1, value) * pow(M_PI * angle_rad, 2 * value);
    den = fact(2 * value);
    num /= den;
    result += num;
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The result is %Lf\n", result);
  printf("The time spent is %f\n", time_spent);
}

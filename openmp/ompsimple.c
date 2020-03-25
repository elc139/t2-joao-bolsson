#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  double *a;
  double *b;
  double c;
} dotdata_t;

/*
 * Tempo (wallclock) em microssegundos
 */
long wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */
void fill(double *a, int size, double value)
{
  int i;
  for (i = 0; i < size; i++)
  {
    a[i] = value;
  }
}

int main(int argc, char **argv)
{
  dotdata_t dotdata;

  int nthreads, wsize, repeat;
  long start_time, end_time;

  if ((argc != 3))
  {
    printf("Uso: %s <worksize> <repetitions>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  nthreads = omp_get_num_threads();
  wsize = atoi(argv[1]);  // worksize = tamanho do vetor de cada thread
  repeat = atoi(argv[2]); // numero de repeticoes dos calculos (para aumentar carga)

  printf("num de threads: %d", nthreads);
  // Cria vetores
  dotdata.a = (double *)malloc(wsize * nthreads * sizeof(double));
  fill(dotdata.a, wsize * nthreads, 0.01);
  dotdata.b = (double *)malloc(wsize * nthreads * sizeof(double));
  fill(dotdata.b, wsize * nthreads, 1.0);
  dotdata.c = 0.0;

  int end = wsize * nthreads;

  start_time = wtime();
#pragma omp parallel shared(dotdata, repeat, end)
  {
    int i, k;
    int offset = omp_get_thread_num();
    double *a = dotdata.a;
    double *b = dotdata.b;
    double mysum;

    for (k = 0; k < repeat; k++)
    {
      mysum = 0.0;
      for (i = 0; i < end; i++)
      {
        mysum += (a[i] * b[i]);
      }
    }

    dotdata.c += mysum;
  }

  end_time = wtime();

  // Mostra resultado e estatisticas da execucao
  printf("%f\n", dotdata.c);
  printf("%d thread(s), %ld usec\n", nthreads, (long)(end_time - start_time));
  fflush(stdout);

  free(dotdata.a);
  free(dotdata.b);
  return EXIT_SUCCESS;
}
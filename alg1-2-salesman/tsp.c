/*
  gcc -x c -std=c99 -Ofast tsp.c
*/


#include <stdlib.h> /* malloc, rand, srand */
#include <stdio.h>  /* printf, fprintf */
#include <time.h>   /* time, clock */
#include <unistd.h> /* getopt */
#include <string.h> /* memcpy */

#define N_CITIES_MAX sizeof (unsigned long) * 8
#define N_CITIES_MIN 2
#define COST_LOWER 1
#define COST_UPPER 99
#define N_CITIES 4
#define START_CITY 0
#define PAD_WIDTH 2
#define N_RUNS 3

inline void swap (int * const a, int * const b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

inline int sln_get_cost (
  size_t const n,
  int const (* const restrict cmat) [n],
  int const * const restrict sln
)
{
  int sum = 0;
  for (size_t i = 0; i < n; ++i) {
    sum += cmat[sln[i]][sln[i+1]];
  }
  return sum;
}

void sln_print (
  size_t const n,
  int const * const restrict sln
)
{
  for (size_t i = 0; i < n; ++i) {
    printf ("%d, ", sln[i] + 1);
  }
  printf ("%d", sln[n] + 1);
}

void cmat_fill_man (
  size_t const n,
  int (* const restrict cmat) [n]
)
{
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      scanf ("%d", &cmat[i][j]);
    }
  }
}

void cmat_fill_rng (
  size_t const n,
  int (* const restrict cmat) [n],
  int const lower,
  int const upper
)
{
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      cmat[i][j] = rand () % (upper - lower) + lower;
      cmat[j][i] = rand () % (upper - lower) + lower;
    }
    cmat[i][i] = 0;
  }
}

void cmat_print (
  size_t const n,
  int const (* const restrict cmat) [n]
)
{
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      printf ("%*d  ", PAD_WIDTH, cmat[i][j]);
    }
    printf ("\n");
  }
}

clock_t find_exact_sln (
  size_t const n,
  int const (* const restrict cmat) [n],
  int const starting_city,
  int * const restrict exact_sln,
  int * const restrict worst_sln,
  int * const exact_sln_cost,
  int * const worst_sln_cost
)
{
  int worst_cost, exact_cost, permut_cost;

  int * const permut_sln = malloc ((n + 1) * sizeof (int));

  if (permut_sln == NULL) { return (clock_t) 0; /* check check */ }

  size_t k, l;

  clock_t total, start;

  for (int i = 1, j = 0; i < n ; ++i, ++j) {
    permut_sln[i] = (j == starting_city) ? ++j : j;
  }
  permut_sln[0] = permut_sln[n] = starting_city;

  permut_cost = sln_get_cost (n, cmat, permut_sln);

  worst_cost = 0;
  exact_cost = n * COST_UPPER + 1;
  
  start = clock ();

  while (1) {
  
    if (worst_cost < permut_cost) {
      worst_cost = permut_cost;
      memcpy (worst_sln, permut_sln, (n + 1) * sizeof (int));
    }

    if (exact_cost > permut_cost) {
      exact_cost = permut_cost;
      memcpy (exact_sln, permut_sln, (n + 1) * sizeof (int));
    }
  
    for (k = n - 1; k != 1 && permut_sln[k - 1] >= permut_sln[k]; --k);

    --k;

    if (k == 0) break;

    for (l = n - 1; permut_sln[l] <= permut_sln[k]; --l);

    swap (&permut_sln[k], &permut_sln[l]);

    for (++k, l = n - 1; l > k; --l, ++k) {
      swap (&permut_sln[k], &permut_sln[l]);
    }

    permut_cost = sln_get_cost (n, cmat, permut_sln);
  }
  
  *exact_sln_cost = exact_cost;
  *worst_sln_cost = worst_cost;
  
  total = clock () - start;

  free (permut_sln);

  return total;
}

clock_t find_apprx_sln (
  size_t const n,
  int const (* const restrict cmat) [n],
  int const starting_city,
  int * const restrict apprx_sln,
  int * const apprx_sln_cost
)
{
  clock_t start, total;
  
  int cmin;
  
  unsigned long cset = 0;
  
  for (unsigned long i = 0; i < n; ++i) { cset |= (unsigned long) 1 << i; }
  
  apprx_sln[0] = starting_city;
  
  cset ^= (unsigned long) 1 << (unsigned long) starting_city;
  
  start = clock ();
  
  for (int i = 1; cset; ++i) {
    unsigned long k, kmin;
    cmin = COST_UPPER + 1;
    for (k = 0; k < n; ++k) {
      if (cset & (unsigned long) 1 << k && cmin > cmat[apprx_sln[i-1]][k])
        { cmin = cmat[apprx_sln[i-1]][k]; kmin = k;}
    }
    apprx_sln[i] = (int) kmin;
    cset ^= (unsigned long) 1 << kmin;
  }
  
  apprx_sln[n] = starting_city;
  
  *apprx_sln_cost = sln_get_cost (n, cmat, apprx_sln);
  
  total = clock () - start;
  
  return total;
}

int main (int argc, char ** argv) {
  size_t n = N_CITIES;
  int starting_city = START_CITY;
  int lower = COST_LOWER;
  int upper = COST_UPPER;

  int c;
  while ((c = getopt (argc, argv, "n:s:")) != -1) {
    switch (c) {
      case 'n':
        n = (size_t) atoi (optarg);
        break;
      case 's':
        starting_city = atoi (optarg) - 1;
        break;
      case '?':
        printf ("Usage: $ tsp [-n n_cities] [-s starting_city_n]\n");
        return -1;
        break;
    }
  }

  if (n < N_CITIES_MIN || n > N_CITIES_MAX) {
    fprintf (stderr, "Invalid N of cities. Expected at least 2 bla bla\n");
    return -1;
  }

  if (starting_city < 0 || starting_city > n - 1) {
    fprintf (stderr, "Invalid starting city index. Expected [1; N]\n");
    return -1;
  }

  int * const cmat      = malloc ( n * n  * sizeof (int));
  int * const worst_sln = malloc ((n + 1) * sizeof (int));
  int * const exact_sln = malloc ((n + 1) * sizeof (int));
  int * const apprx_sln = malloc ((n + 1) * sizeof (int));

  if (!(cmat && worst_sln && exact_sln && apprx_sln)) {
    fprintf (stderr, "Could not allocate memory\n");
    return -1;
  }

  srand (time (NULL));

  int exact_cost, worst_cost, apprx_cost;
  
  double avgt_e = 0, avgt_a = 0;
  double avgcp = 0;
  double avgtp = 0;
  int n_runs = N_RUNS;
  
  printf ("------------ N cities: %llu ---- ", n);
  printf ("Cost range: [%d; %d] -----------\n", lower, upper);
  
  for (int i = 0; i < n_runs; ++i) {
    clock_t e, a;
    double te, ta;
    double cp;
    double tp;
    printf ("-------- Test run %d out of %d --------\n", i+1, n_runs);
    
    cmat_fill_rng (n, cmat, lower, upper);
    
    e = find_exact_sln (n, cmat, starting_city, exact_sln, worst_sln, \
                        &exact_cost, &worst_cost);
    a = find_apprx_sln (n, cmat, starting_city, apprx_sln, &apprx_cost);
    avgt_e += te = (double) e / (double) CLOCKS_PER_SEC;
    avgt_a += ta = (double) a / (double) CLOCKS_PER_SEC;
    avgcp += cp = 1.0 - (double) (apprx_cost - exact_cost) / \
                      (double) (worst_cost - exact_cost);
    avgtp += tp = te / ta;
    
    printf ("Cost matrix:\n"); cmat_print (n, cmat);
    
    printf ("Exact alg. solution: "); sln_print(n, exact_sln);
    printf ("\nExact sln. cost: %d\n", exact_cost);
    printf ("Worst sln. cost: %d\n", worst_cost);
    printf ("Exact alg. run-time: %.3f s\n\n", te);
    
    printf ("Heur. alg. solution: "); sln_print(n, apprx_sln);
    printf ("\nHeur. sln. cost: %d\n", apprx_cost);
    printf ("Heur. alg. run-time: %.3f s\n\n", ta);
    
    printf ("How good is heur. sln. compared to exact. sln: %.1f %% \n", cp * 100.0);
    printf ("How fast is heur. alg. compared to exact. alg: %.1f %% \n\n", tp * 100.0);
  }
  avgt_a /= (double) n_runs;
  avgt_e /= (double) n_runs;
  avgcp /= (double) n_runs;
  avgtp /= (double) n_runs;
  
  printf ("-------- Averages out of %d runs ---------------\n", n_runs);
  printf ("Average run-time of exact alg: %.3f s \n", avgt_e);
  printf ("Average run-time of heur. alg: %.3f s \n", avgt_a);
  printf ("Average goodness of heur. alg: %.1f %% \n", avgcp * 100.0);
  printf ("Average fastness of heur. alg: %.1f %% \n\n", avgtp * 100.0);
  
  return 0;
}

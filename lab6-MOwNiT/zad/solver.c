#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>

double random_double()
{
    long divident = random(), divisor = random(), sign = random() % 2;
    double res = (double) divident / (double) divisor;
    if (!sign) {
        res = -res;
    }
    return res;
}

double multiply_ith_matrix_row_by_vector(int i, long n, gsl_matrix* a, gsl_vector* x)
{
    double res = 0;
    for (int j = 0; j < n; j++) {
        res += gsl_matrix_get(a, i, j) * gsl_vector_get(x, j);
    }
    return res;
}

double time_between(struct rusage *ru0, struct rusage *ru1)
{
    double utime, stime;

    // calculate user time and system time
    utime = (double) ru1->ru_utime.tv_sec
            + 1.e-6 * (double) ru1->ru_utime.tv_usec
            - ru0->ru_utime.tv_sec
            - 1.e-6 * (double) ru0->ru_utime.tv_usec;
    stime = (double) ru1->ru_stime.tv_sec
            + 1.e-6 * (double) ru1->ru_stime.tv_usec
            - ru0->ru_stime.tv_sec
            - 1.e-6 * (double) ru0->ru_stime.tv_usec;
    return stime + utime;
}

int main (int argc, char** args)
{
    if (argc != 2)
    {
        printf("Usage: ./solver n (where n is the side of square matrix)");
        return 1;
    }
    long n = strtol(args[1], NULL, 10);
    if (n < 1)
    {
        printf("Side of the square matrix must be at least 1");
        return 2;
    }
    srandom(time(NULL));

    // allocate memory for matrices and vectors
    gsl_matrix *a = gsl_matrix_alloc(n, n);
    gsl_matrix *a_copy = gsl_matrix_alloc(n, n); // needed to check if solution is correct as 'a' will be destroyed
    gsl_vector *b = gsl_vector_alloc(n);
    gsl_vector *b_recalculated = gsl_vector_alloc(n);
    gsl_vector *x = gsl_vector_alloc(n);
    gsl_permutation *p = gsl_permutation_alloc(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            gsl_matrix_set(a, i, j, random_double());
            gsl_matrix_set(a_copy, i, j, gsl_matrix_get(a, i, j));
        }
    }
    for (int i = 0; i < n; i++)
    {
        gsl_vector_set(b, i, random_double());
    }
    int s;
    struct rusage t0, t1, t2;

    // decompose and solve
    getrusage(RUSAGE_SELF, &t0);
    gsl_linalg_LU_decomp(a, p, &s);
    getrusage(RUSAGE_SELF, &t1);
    gsl_linalg_LU_solve(a, p, b, x);
    getrusage(RUSAGE_SELF, &t2);

    // write time of decomposition and solving to a file
    FILE* out_decomp = fopen("decomp_times.txt","a");
    FILE* out_solve = fopen("solve_times.txt","a");
    fprintf(out_decomp, "%ld %3f\n", n, time_between(&t0, &t1));
    fprintf(out_solve, "%ld %3f\n", n, time_between(&t1, &t2));


    printf("Solution vector x = \n");
    gsl_vector_fprintf (stdout, x, "%g");
    printf("Original vector b = \n");
    gsl_vector_fprintf (stdout, b, "%g");
    // calculate b = A * x
    for (int i = 0; i < n; i++)
    {
        gsl_vector_set(b_recalculated, i, multiply_ith_matrix_row_by_vector(i, n, a_copy, x));
    }
    printf("Recalculated vector b = \n");
    gsl_vector_fprintf (stdout, b_recalculated, "%g");

    // free allocated memory
    gsl_permutation_free(p);
    gsl_vector_free(b);
    gsl_vector_free(x);
    gsl_matrix_free(a);
    return 0;
}
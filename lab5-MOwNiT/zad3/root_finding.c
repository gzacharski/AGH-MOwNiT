/*
Program w zaleznosci od parametru programu stosuje metode Newtona, uproszczoną metodę Newtona lub metodę Steffensena,
czyli odpowiednio: gsl_root_fdfsolver_newton, gsl_root_fdfsolver_secant lub gsl_root_fdfsolver_steffenson
dla rownania x^2 - 2x + 1 = 0.

Rozwiazaniem jest x = 1.
*/
#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include "string.h"

#include "demo_fn.h"

int main (int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage: ./root_solving [newton|simplified_newton|steffensen]\n");
        return 1;
    }
    char* method = argv[1];
    int status;
    // ustalona maksymalna ilosc iteracji
    int iter = 0, max_iter = 100;
    const gsl_root_fdfsolver_type *T;
    gsl_root_fdfsolver *s;
    // pierwiastek poprzedni, aktualny i oczekiwany (wyliczony recznie)
    double r0, r1 = 5.0, r_expected = 1.0;
    gsl_function_fdf F;
    // f(x) = x^2 - 2x + 1
    struct quadratic_params params = {1.0, -2.0, 1.0};

    // ustawione: funkcja obliczajaca wartosc funkcji kwadratowej w punkcie x,
    // pochodna tej funkcji oraz obie wartosci naraz
    F.f = &quadratic;
    F.df = &quadratic_deriv;
    F.fdf = &quadratic_fdf;
    F.params = &params;

    // wybor odpowiedniej metody przez parametr programu
    if(strcmp(method, "newton") == 0)
    {
        T = gsl_root_fdfsolver_newton;
    }
    else if(strcmp(method, "simplified_newton") == 0)
    {
        T = gsl_root_fdfsolver_secant;
    }
    else if(strcmp(method, "steffensen") == 0)
    {
        T = gsl_root_fdfsolver_steffenson;
    }
    else
    {
        printf("Usage: ./root_solving [newton|simplified_newton|steffensen]\n");
        return 2;
    }
    s = gsl_root_fdfsolver_alloc (T);
    gsl_root_fdfsolver_set (s, &F, r1);

    printf ("using %s method\n",
          gsl_root_fdfsolver_name (s));

    printf ("%5s %9s %10s %9s\n",
          "iter", "root", "err", "err(est)");

    do
    {
        iter++;
        status = gsl_root_fdfsolver_iterate (s);
        r0 = r1;
        r1 = gsl_root_fdfsolver_root (s);
        // sprawdzenie czy pierwiastek jest obliczony z wystarczajaca precyzja
        status = gsl_root_test_delta(r1, r0,0, 0.001);

        if (status == GSL_SUCCESS) printf ("Converged:\n");

        printf ("%5d %.7f %+.7f %.7f\n",
              iter, r1, r1 - r_expected, r1 - r0);
    }
    while (status == GSL_CONTINUE && iter < max_iter);

    gsl_root_fdfsolver_free(s);
    return status;
}



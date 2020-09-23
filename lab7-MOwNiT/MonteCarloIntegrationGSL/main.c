#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_qrng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

double myFunction1 (double *x, size_t dim, void *params){
  (void)(dim);
  (void)(params);
  return 1/sqrt(x[0]);
}

double myFunction2 (double *x, size_t dim, void *params){
  (void)(dim);
  (void)(params);
  return x[0]*x[0];
}

void display_results (char *title, double result, double error, double exact){
  printf ("%s ==================\n", title);
  printf ("result = % .6f\n", result);
  printf ("sigma  = % .6f\n", error);
  printf ("exact  = % .6f\n", exact);
  printf ("error  = % .6f = %.2g sigma\n", result - exact, fabs (result - exact) / error);
}

void calculateMonteCarloPlain(gsl_monte_function * f, const double xl[], const double xu[],
        size_t dim, size_t calls, gsl_rng * r, double result, double abserr, double exact, FILE *file){

    gsl_monte_plain_state *s = gsl_monte_plain_alloc (dim);
    gsl_monte_plain_integrate (f, xl, xu, dim, calls, r, s, &result, &abserr);
    gsl_monte_plain_free (s);

    display_results ("plain", result, abserr, exact);
    fprintf(file,"%d\t%f\t%f\n",calls,result,abserr);
}

void calculateMonteCarloMiser(gsl_monte_function * f, const double xl[], const double xu[],
        size_t dim, size_t calls, gsl_rng * r, double result, double abserr, double exact, FILE *file){

    gsl_monte_miser_state *s = gsl_monte_miser_alloc(dim);
    gsl_monte_miser_integrate (f, xl, xu, dim, calls, r, s, &result, &abserr);
    gsl_monte_miser_free (s);

    display_results ("miser", result, abserr, exact);
    fprintf(file,"%d\t%f\t%f\n",calls,result,abserr);
}

void calculateMonteCarloVegas(gsl_monte_function * f, double xl[], double xu[],
        size_t dim, size_t calls, gsl_rng * r, double result, double abserr, double exact, FILE *file){

    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc (dim);
    gsl_monte_vegas_integrate (f, xl, xu, dim, 10000, r, s, &result, &abserr);
    display_results ("vegas warm-up", result, abserr, exact);

    printf ("converging...\n");

    do
      {
        gsl_monte_vegas_integrate (f, xl, xu, dim, calls/5, r, s, &result, &abserr);
        printf ("result = % .6f sigma = % .6f chisq/dof = %.1f\n", result, abserr, gsl_monte_vegas_chisq (s));
      }
    while (fabs (gsl_monte_vegas_chisq (s) - 1.0) > 0.5);

    display_results ("vegas final", result, abserr, exact);
    fprintf(file,"%d\t%f\t%f\n",calls,result,abserr);

    gsl_monte_vegas_free (s);
}

int main(int argc, char** argv){

    if(argc!=4){
        printf("Usage: myFunction1|myFunction2 Plain|Miser|Vegas iterations.\n");
        return 1;
    }

    gsl_monte_function F;//funkcja
    char *chosenFunction=argv[1];
    char *chosenMethod=argv[2];
    double exact;
    FILE *outputIntegralValue;
    char filename[100]="";

    if(strcmp(chosenFunction,"myFunction1")==0){
        F.f=&myFunction1;
        F.dim=1;
        F.params=0;
        exact = 2;
        strcat(filename,"outputForFunction1GSL");

    }else if(strcmp(chosenFunction,"myFunction2")==0){
        F.f=&myFunction2;
        F.dim=1;
        F.params=0;
        exact = ((double)1)/3;
        strcat(filename,"outputForFunction2GSL");

    }else{
        printf("Usage: myFunction1|myFunction2 Plain|Miser|Vegas iterations.\n");
        return 2;
    }

    double xl[1] = { 0 }; //dolna granica przedzia³u
    double xu[1] = { 1 }; //górna granica przedzia³u

    size_t dimension=1; //funkcja jednej zmiennej
    size_t calls = atoi(argv[3]); //iloœæ iteracji

    const gsl_rng_type *T; //random number generator typ
    gsl_rng *r;// random number generator

    gsl_rng_env_setup ();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    double res, err; //miejsce na wynik oraz wynik b³êdów
    int i;

    if(strcmp(chosenMethod,"Plain")==0){

        strcat(filename,"methodPlain.txt");
        outputIntegralValue=fopen(filename,"a");

        for(i=100;i<calls;i++){
            calculateMonteCarloPlain(&F, xl, xu, dimension, i, r, res, err, exact, outputIntegralValue);
        }

    }else if(strcmp(chosenMethod,"Miser")==0){

        strcat(filename,"methodMiser.txt");
        outputIntegralValue=fopen(filename,"a");

        for(i=100;i<calls;i++){
            calculateMonteCarloMiser(&F, xl, xu, dimension, i, r, res, err, exact, outputIntegralValue);
        }

    }else if(strcmp(chosenMethod,"Vegas")==0){

        strcat(filename,"methodVegas.txt");
        outputIntegralValue=fopen(filename,"a");

        for(i=100;i<calls;i++){
            calculateMonteCarloVegas(&F, xl, xu, dimension, i, r, res, err, exact, outputIntegralValue);
        }
    }

    gsl_rng_free (r);

    return 0;
}



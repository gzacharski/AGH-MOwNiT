#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_qrng.h>
#include <math.h>
#include <time.h>

double myFunction1(double x){
    return 1/sqrt(x);
}

double myFunction2(double x){
    return x*x;
}

double calculateIntegral(double (*yourFunction)(double), double lowerBound,
                         double upperBound, unsigned int iterations){

    if(upperBound<=lowerBound) return -1;

    double sum=0;
    int i;
    //liczby quasipseudolosowe
    /*
    double randomNumber[1];
    gsl_qrng *quasiRandomNumbers=gsl_qrng_alloc(gsl_qrng_halton,1);
    gsl_qrng_get(quasiRandomNumbers,randomNumber);
    gsl_qrng_free(quasiRandomNumbers);

    for(i=0; i<iterations; i++){
       sum+=yourFunction(randomNumber[0]*upperBound+lowerBound);
    }
    */

    //liczby pseudolosowe

    double randomNumbersArray[iterations];
    for(i=0; i<iterations; i++){
        randomNumbersArray[i]=((double)rand()/RAND_MAX)*upperBound+lowerBound;
    }
    for(i=0; i<iterations; i++){
       sum+=yourFunction(randomNumbersArray[i]*upperBound+lowerBound);
    }

    double calculatedIntegral=(upperBound-lowerBound)*sum/iterations;

    return calculatedIntegral;
}

int main(int argc, char** argv){

    srand((unsigned int)time(NULL));

    if(argc!=5){
        printf("Usage: myFunction1|myFunction2 lowerBound upperBound numberOfIterations.\n");
        return 1;
    }else if(atoi(argv[4])<10){
        printf("Usage: numberOfIterations must equal at least 10.\n");
        return 3;
    }

    char *chosenFunction=argv[1];
    double lowerBound=atof(argv[2]);
    double upperBound=atof(argv[3]);
    int iterations=atoi(argv[4]);
    double correctValue;

    FILE *outputIntegralValue;
    double (*functioToCalculate)(double);

    if(strcmp(chosenFunction,"myFunction1")==0){
        functioToCalculate=myFunction1;
        correctValue=2; //tylko dla przedzialu 0 1
        outputIntegralValue=fopen("outputForFunction1.txt","a");
    }else if(strcmp(chosenFunction,"myFunction2")==0){
        functioToCalculate=myFunction2;
        correctValue=((double)1)/3; //tylko dla przedzialu 0 1
        outputIntegralValue=fopen("outputForFunction2.txt","a");
    }else{
        printf("Usage: myFunction1|myFunction2\n");
        return 2;
    }
    printf("For: %s %f %f %d\n",chosenFunction,lowerBound,upperBound,iterations);

    int i;
    for(i=10;i<iterations;i++){
        double result=calculateIntegral(functioToCalculate,lowerBound, upperBound, i);
        fprintf(outputIntegralValue,"%d\t%.16f\t%.16f\n",i,result,correctValue-result);
        printf("Result:%d\t%.16f\t%.16f\n",i,result,correctValue-result);
    }

    return 0;
}

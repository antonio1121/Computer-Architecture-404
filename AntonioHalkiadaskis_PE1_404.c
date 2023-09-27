#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mm_malloc.h>
#include <omp.h>
#define ELEMENTS 1000 // Amount of elements in array
#define OUTERLOOP 3000 // Outer-loop for accuracy
#ifndef RAND_MAX // Defines RAND MAX if it does not exist in the macros
#define RAND_MAX ((int) ((unsigned) ~0 >> 1))
#endif

// Antonio Halkiadakis ICSI404 Project 1

	int main() {

		// Initialize variables to be used in array operations
		int i,j ;
		double intial, final ;
		srand((unsigned)time(NULL));
		double* x = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));
		double* y = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));
		double* z = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));

		if(x==NULL) {
			printf("uhoh, memory allocation failed");
		}

		// Populates arrays with random values between 0 to 1000 inclusive

		for(i = 0; i!=ELEMENTS;i++) {
			x[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
		}

		for(i = 0; i!=ELEMENTS;i++) {
			y[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
		}

		for(i = 0; i!=ELEMENTS;i++) {
			z[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
		}

		i = 0 ; // Sets counter to 0 from previous loop

		// Operations are done here, with the time set in place.
		intial = omp_get_wtime();

		for(j = 0; j!=OUTERLOOP;j++) {

			for(i = 0;i!=ELEMENTS;i++) {
				z[i] = x[i] / y[i] ;
			}
		}

		final = omp_get_wtime();
 
		printf("The time for %d operations took %f second(s).\n",ELEMENTS * OUTERLOOP,final-intial);

		return 0 ;
	}
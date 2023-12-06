#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mm_malloc.h>
#include <omp.h>
#define ELEMENTS 128 // Amount of elements in array
#define OUTERLOOP 10000000 // Outer-loop for accuracy
#define CLOCKRATE 2.625E9 // Clock rate for an Intel Xeon 6338 (2.625GHZ if high load, 3.6GHZ if low load)
#ifndef RAND_MAX // Defines RAND MAX if it does not exist in the macros
#define RAND_MAX ((int) ((unsigned) ~0 >> 1))
#endif

// Antonio Halkiadakis ICSI404 Project 3

	int main() {

		// Initialize variables to be used in array operations

		int i,j ;
		double initial, final ;
		srand((unsigned)time(NULL));

		double* x = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));
		double* y = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));
		double* z = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));
		double* a = (double*) _mm_malloc(sizeof(double) * ELEMENTS,sizeof(double));

		if(x==NULL || y==NULL || z==NULL || a==NULL) {
			printf("Uh-oh, memory allocation failed.\n");
			return 1 ;
		}

		// Populates arrays with random values between 0 to 1000 inclusive

		for(i = 0; i!=ELEMENTS;i++) {

			x[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
			a[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
			y[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
			z[i] = ((double) rand() / ((double) RAND_MAX + 1 )) * (1000);
		}

		i = 0 ; // Sets counter to 0 from previous loop

		// Operations are done here, with the time set in place.
		
		initial = omp_get_wtime();

		for(j = 0; j!=OUTERLOOP;j++) {

			for(i = 0;i!=ELEMENTS;i++) {
				z[i] = (a[i] * x[i]) + y[i];
			}
		}

		final = omp_get_wtime();

		_mm_free(x);
		_mm_free(y);
		_mm_free(z);
		_mm_free(a);

		double timeElapsed = final-initial;
		double numberOfElements = ELEMENTS * OUTERLOOP;
		double element = timeElapsed / numberOfElements;
		double vector = timeElapsed / OUTERLOOP;
		double flops = 1 / element;
		double opsPerSec = numberOfElements / timeElapsed ;
		double cpo = CLOCKRATE / flops;
		double opc = 1 / cpo ;
		double dataTransfer = flops * 8 * 4 ;
		double peakOps = 8 * CLOCKRATE ;
		double util = (opsPerSec / peakOps) * 100 ;

		printf("\n\033[0;31mDAXPY benchmark\033[0;37m\n\n");
		printf("\nNumber of elements: %d\n",ELEMENTS);
		printf("Number of outer loops: %d\n",OUTERLOOP); 
		printf("The time for %g operations took %0.4f second(s).\n",numberOfElements,timeElapsed);
		printf("1 element takes %0.2e second.\n",element);
		printf("One array takes %0.2e second.\n",vector);
		printf("Operations per second: %0.2e per sec\n",opsPerSec);
		printf("Cycles per operation: %0.2f\n",cpo);
		printf("Operations per cycle: %0.2f\n",opc);
		printf("Estimated data transfer to and from the CPU: %0.2e Bytes/sec\n",dataTransfer);
		printf("%% Utilization of peak operations per sec(%g): %0.2f%%\n\n",peakOps,util);

		return 0 ;
	}

	// Time / (Instructions * Cycles/Instruction * 1/Clock Rate) = CPI
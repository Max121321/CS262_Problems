#include <stddef.h>  
#include <stdio.h>
#include <stdlib.h>    
#include <math.h>
#include "omp.h" 

int main(int argc, char *argv[]) {
    double start, end;
    start = omp_get_wtime(); //start time

    double x1 = atof(argv[1]); // Limits of integration, lower x
    double x2 = atof(argv[2]); // Limits of integration, upper x
    size_t nsteps = atol(argv[3]); // Number of steps in Riemann Sum

    double dx = (x2 - x1) / nsteps; // delta x for the Riemann sum
    double ysum = 0.0;  // Temporary variable to hold the sum prior to multiplication by dx

    //using the reduction here is essential to computing our total sum. the '(+:ysum)' saves
    //the value in ysum for that unique thread. the for loop breaks up the work between the threads and
    //the ysum sums all the sums from all the threads to get our integral
#pragma omp parallel for reduction(+:ysum)
    for (size_t i = 0; i < nsteps; i++) {
        double x = x1 + i * dx; //x value at this step
        ysum += sin(x); //summation of y(x)
    }

    double integral = ysum * dx;    // Our computed integral: the summation of y(x)*dx
    double analytic = -cos(x2) + cos(x1);   // The known, exact answer to this integration problem

    end = omp_get_wtime(); //time stop
  
   
   printf("Function: y(x) = sin(x) [note that x is in radians]\n");
   printf("Limits of integration: x1 = %lf, x2 = %lf \n", x1, x2);
   printf("Riemann sum with %ld steps, a.k.a. dx=%lf \n", nsteps, dx); 
   printf("Computed integral: %lf \n", integral);
   printf("Exact integral:    %lf \n", analytic);
   printf("Percent error:     %lf %% \n", fabs((integral - analytic) / analytic)*100);
   printf("Work took %f milliseconds\n", 1000 * (end - start));
   return 0;
}
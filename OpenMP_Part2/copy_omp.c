#include <stdio.h>
#include <stdlib.h> 
#include <omp.h>

#define NUM_BOOKS 2188

int main(int argc, char* argv[]) {

    double t0 = omp_get_wtime(); //start time

    int bookid;
    int length;
    length = snprintf(NULL, 0, "cp books/%05d.txt books2/%05d.txt", 0, 0);



#pragma omp parallel //parallel implementation
    {

        char* command = malloc(length + 1);  //allocating memory per thread

        //breaks up work dynamically as we dont now how hard it is to copy a txt
        //this assures that a work load isn't unbalnced between threads
#pragma omp for schedule(dynamic)
        for (bookid = 0; bookid < NUM_BOOKS; bookid++) {

            snprintf(command, length + 1, "cp books/%05d.txt books2/%05d.txt", bookid, bookid);

            system(command);
            printf("Copying 'books/%05d.txt' to 'books2/%05d.txt'\n", bookid, bookid);
        }
    }

    double t1 = omp_get_wtime(); //end time
    printf("Seconds elapsed: %f\n", t1 - t0);

    return(0);
}
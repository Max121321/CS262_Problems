#include <unistd.h> // sleep()
#include <stdio.h>
#include <omp.h>

int walk(int secs);
int chew_gum(int secs);

int main() {
    double t0 = omp_get_wtime(); // Start timer
    omp_set_num_threads(2);  //HARD-CODED threads numbers

#pragma omp parallel    //parallel implementation
    {
    int id = omp_get_thread_num();  //set current thread # to @id
   
  //Checking if it's even or odd using modulo %2 to break up work
    if (id % 2 == 0) {  
        walk(5); 
        }
    if (id % 2 != 0) {  
        chew_gum(5); 
         }
    }
    double t1 = omp_get_wtime(); // End timer
    printf("Seconds elapsed: %f\n", t1 - t0);
    return 0;
}

// DO NOT CHANGE CODE BELOW THIS LINE
int walk(int secs) {
    int i;
    for (i=0; i < 5; i++) {
        printf("Walking...\n");
        sleep(1);
    }
    return 0;
}

int chew_gum(int secs) {
    int i;
    for (i=0; i < 5; i++) {
        printf("Chewing bubblegum...\n");
        sleep(1);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h> 
#include <omp.h>

#define NUM_BOOKS 2188 

int main (int argc, char *argv[]) {
   double t0 = omp_get_wtime(); //start time
   int length; 
   length = snprintf(NULL, 0, "gzip -%d < books/%05d.txt > books2/%05d.txt.gz", 0, 0, 0);

   int compression_level = atoi(argv[1]); 

#pragma omp parallel
   {
       char* command = malloc(length + 1); //allocates momory per thread

       //using dynamic is essential as the sizes of the files are different so
       //splitting the work based on completing of the file is essential
       #pragma omp for schedule(dynamic) 
       for (int bookid = 0; bookid < NUM_BOOKS; bookid++) {
           snprintf(command, length+1, "gzip -%d < books/%05d.txt > books2/%05d.txt.gz", compression_level, bookid, bookid);
    
           system(command);
           printf("Compressing 'books/%05d.txt', Compression level %d. Creating 'books2/%05d.txt.gz'\n", bookid, compression_level, bookid);
       }
   }


   double t1 = omp_get_wtime(); //end time
   printf("Seconds elapsed: %f\n", t1 - t0);
   return(0);
} 
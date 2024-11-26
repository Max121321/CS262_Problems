#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_BOOKS 2188 

int tally_book(int bookid, int* tally);

int main() {
    double t0 = omp_get_wtime(); //start time

    int global_tally[27] = {}; //global tally initialized to 0
    int length = snprintf(NULL, 0, "books/%05d.txt", 0);

#pragma omp parallel
    {

        char* fname = malloc(length + 1);   //allocating memory per thread

        //using dynamic scheduling allows the threads to do work based on load
        //adding the reduction allows for the global_tally count to be saved
        //across all the threads
#pragma omp for schedule(dynamic) reduction(+:global_tally[:27])
        //loops through the assigned range from @start and @end
        for (int bookid = 0; bookid < NUM_BOOKS; bookid++) {

            // Tally all characters in this book into the 27-element array 'tally'
            snprintf(fname, length + 1, "books/%05d.txt", bookid);
            printf("Reading file: %s\n", fname);

            // Read this file letter-by-letter, and tally accordingly
            FILE* fptr = fopen(fname, "r");

            int ch;
            while ((ch = fgetc(fptr)) != EOF) {
                int letter;
                // Categorize the character as one of 26 letters, or a non-letter
                if (ch > 'A' - 1 && ch < 'Z' + 1) {
                    letter = ch - 'A'; // Uppercase letter; index 0 - 25
                }
                else if (ch > 'a' - 1 && ch < 'z' + 1) {
                    letter = ch - 'a'; // Lowercase letter; index 0 - 25
                }
                else {
                    letter = 26; // Not a letter; index 26
                }
                // Tally the local character into the appropriate bin
                global_tally[letter] += 1;
            }
            fclose(fptr);
        }
        free(fname);
        //merges the local tally counts into the global tally using omp critical
        //this is effective at preventing race conditions
    }
    // Print the total tally
    for (int i = 0; i < 26; i++) {
        printf("%c%c: %d\n", i + 'A', i + 'a', global_tally[i]);
    }

    printf("Other: %d\n", global_tally[26]);
    double t1 = omp_get_wtime();  // End timer
    printf("Milliseconds elapsed: %f\n", 1000 * (t1 - t0));
    return 0;
}
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_THREADS 40 //# of threads

#define ARRAY_SIZE 1000000 //# of lines you want to compute; must be evenly divisible by NUM_THREADS!
#define LINE_SIZE 10000 //# of char of each line you want to compute

//MAKE GLOBAL VARIABLE
int int_array[ARRAY_SIZE];


//INITIALIZE THE VARIABLE(S)
void init_arrays()
{
    //SET VARIABLES
    int i;

    //INTIALIZES ALL TO ZERO
    for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	    int_array[i] = 0;
    }
  
}


// Calculates the highest ASCII per line
// and stores the result in the global variable
void *calc_highest(int myID)
{
    // Establish variables
    int i, j, nchars, highest_ascii;
    int startPos = myID * (ARRAY_SIZE / omp_get_num_threads());
    int endPos = startPos + (ARRAY_SIZE / omp_get_num_threads());
    char line[LINE_SIZE + 1];
    int int_temp_array[ARRAY_SIZE] = {0};
    FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
    
    printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);

    //check for bad file
    if (fd == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Get line to the starting position
    for (i = 0; i < startPos; i++) {
        if (fscanf(fd, "%[^\n]\n", line) == EOF) break;
    }

    // Read in lines and calculate highest ASCII
    for (i = startPos; i < endPos; i++) {
        if (fscanf(fd, "%[^\n]\n", line) == EOF) break;
        nchars = strlen(line);
        highest_ascii = 0;
        for (j = 0; j < nchars; j++) {
            if ((int)line[j] > highest_ascii) {
                highest_ascii = (int)line[j];
            }
        }
        int_temp_array[i] = highest_ascii;
    }

    // Close file and transfer result to global variable
    fclose(fd);
    #pragma omp critical
    {
        for (i = startPos; i < endPos; i++) {
            int_array[i] += int_temp_array[i];
        }
    }

    return NULL;
}


//PRINTS ALL THE RESULTS (STORES IN THE A.OUT FILE)
void print_results()
{
    //SETS VARIABLES
    int i;
  
    //PRINTS ALL THE RESULTS
    for ( i = 0; i < ARRAY_SIZE; i++ )  {
       printf("%d: %d\n", i, int_array[i]);
    }
  
  
}


//THE MAIN FUNCTION
int main() {

    //omp_set_num_threads(NUM_THREADS);
	
	//INITIALIZE THE ARRAY
	init_arrays();

    //PERFORMS THE PARALLEL CALCULATIONS
	#pragma omp parallel
	{
        calc_highest(omp_get_thread_num());
	}

    //PRINT THE RESULTS
	print_results();

    //PRINTS EXIT STATEMENT
	printf("Main: program completed. Exiting.\n");
	
	return 0;
}


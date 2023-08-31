#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

int NUM_THREADS; //make sure it divides evenly into ARRAY_SIZE !
                 //defaults to 8

#define ARRAY_SIZE 1000000 //# of lines you want to compute; must be evenly divisible by NUM_THREADS!
#define LINE_SIZE 10000 //# of char of each line you want to compute

//SET UP LOCAL & GLOBAL VARIABLES
int int_array[ARRAY_SIZE];
int int_temp_array[ARRAY_SIZE];


//INITIALIZE ARRAYS
void init_arrays()
{
    //SET UP VARIABLES
    int i;
  
    //PRINT PROMPT
    printf("Initializing arrays.\n"); fflush(stdout);


    //SET ALL EQUAL TO ZERO
    for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	    int_array[i] = 0;
    }
  
}


//CALCULATES THE HIGHEST ASCII PER LINE
void *calc_highest(void *rank)
{
    //SETS UP VARIABLES
    char *line = (char*) malloc( LINE_SIZE + 1 );
    
    // Unpack rank
    int myID = *((int*) rank);

    // Calculate start and end positions
    int startPos = myID * (ARRAY_SIZE / NUM_THREADS);
    int endPos = (myID + 1) * (ARRAY_SIZE / NUM_THREADS);

    // Open file
    FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");

    // Check if file was opened successfully
    if (fd == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Read lines and calculate highest ASCII value
    // line[LINE_SIZE];
    int highest_ascii, nchars;
    for (int i = startPos; i < endPos; i++) {
        if (fgets(line, LINE_SIZE, fd) == NULL) {
            break;
        }
        nchars = strlen(line);
        highest_ascii = 0;
        for (int j = 0; j < nchars; j++) {
            if (line[j] > highest_ascii) {
                highest_ascii = line[j];
            }
        }
        int_temp_array[i] += highest_ascii;
    }

    // Close file
    fclose(fd);
    
    // Free memory
    free(line);

    // Exit thread for MPI
    return NULL;
		
}


//PRINTS THE RESULTS
void print_results()
{
    //SET VARIABLES
    int i;
  
    //PRINTS THE RESULTS
    for ( i = 0; i < ARRAY_SIZE; i++ )  {
       printf("%d: %d\n", i, int_array[i]);
    }
  
  
}


//MAIN FUNCTION
int main(int argc, char* argv[]) {

    //SETS VARIABLES
	int rc;
	int numtasks, rank;
	// MPI_Status Status;

	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	  printf ("Error starting MPI program. Terminating.\n");
          MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	NUM_THREADS = numtasks;
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);

	if ( rank == 0 ) {
		init_arrays();
	}
		
	calc_highest(&rank);

	MPI_Reduce(int_temp_array, int_array, ARRAY_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


	if ( rank == 0 ) {
		print_results();
	}

	MPI_Finalize();
	return 0;
	
}


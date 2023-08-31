#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 1000 //# of lines you want to compute; must be evenly divisible by NUM_THREADS!
#define LINE_SIZE 10000 //# of char of each line you want to compute

//SETS THE MUTEX
pthread_mutex_t mutexsum;


//SETS UP GLOBAL VARIABLES
int int_array[ARRAY_SIZE];
int num;

//INITIALIZES THE GLOBAL VARIABLES
void init_arrays()
{
    //SETS UP LOCAL VARIABLES
    int i;
  
    //INITILIZES MUTEX  
    pthread_mutex_init(&mutexsum, NULL);

    //SETS ALL EQUAL TO ZERO
    for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	    int_array[i] = 0;
    }
  
}


void *calc_highest(void *myID)
{
    int i, j;
    int startPos, endPos;
    int highest_ascii;
    char line[LINE_SIZE + 1];
    FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");

    // check for error opening file
    if (fd == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    startPos = ((intptr_t) myID) * (ARRAY_SIZE / num);
    endPos = startPos + (ARRAY_SIZE / num);
    
    printf("startPos = %d endPos = %d \n", startPos, endPos);
    
    
    // Get line to the starting position
    for (i = 0; i < startPos; i++) {
        if (fscanf(fd, "%[^\n]\n", line) == EOF) break;
    }
    
    
    for (i = startPos; i < endPos; i++) {
        if (fscanf(fd, "%[^\n]\n", line) == EOF) break;
        int nchars = strlen(line);
        highest_ascii = 0;
        for (j = 0; j < nchars; j++) {
            if (line[j] > highest_ascii) {
                highest_ascii = line[j];
            }
        }
        pthread_mutex_lock(&mutexsum);
        
        int_array[i] = highest_ascii;
            
        pthread_mutex_unlock(&mutexsum);
    }
    
    fclose(fd);
    pthread_exit(NULL);
}


//PRINTS THE RESULTS TO A.OUT
void print_results()
{
    //ESTABLISHES VARIABLES
    int i;
  
    //PRINTS THE RESULTS
    for ( i = 0; i < ARRAY_SIZE; i++ )  {
       printf("%d: %d\n", i, int_array[i]);
    }
  
  
}


//THE MAIN FUNCTION
int main(int argc, char* argv[]) {
    
    char *p;
    long conv = strtol(argv[1], &p, 10);
    
    num = argc;
    num = conv;
    
    //SETS UP VARIABLES
    int i, rc;
	pthread_t threads[num];
	pthread_attr_t attr;
	void *status;
	
	
	//INITIALIZE AND SET THREAD DETACHED ATTRIBUTE
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	
	//INITIALIZES ARRAYS
	init_arrays();
	
	//PERFORMS PTHREADS
	for (i = 0; i < num; i++ ) {
    rc = pthread_create(&threads[i], &attr, calc_highest, (void*)(uintptr_t)i);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
}

	

    
    //FREE ATTRIBUTE AND WAIT FOR THE OTHER THREADS
	pthread_attr_destroy(&attr);
	for(i=0; i<num; i++) {
	     rc = pthread_join(threads[i], &status);
	     if (rc) {
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}

    //PRINT THE RESULTS
	print_results();

    //DESTROYS MUTEX, PRINTS EXIT PROMPT, EXITS PTHREAD
	pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
	
	return 0;
	
	
}


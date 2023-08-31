#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_THREADS 5 //# of threads

#define ARRAY_SIZE 1000 //# of lines you want to compute; must be evenly divisible by NUM_THREADS!
#define LINE_SIZE 10000 //# of char of each line you want to compute

//SETS THE MUTEX
pthread_mutex_t mutexsum;


//SETS UP GLOBAL VARIABLES
int int_array[ARRAY_SIZE];


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


//CALCULATES THE HIGHEST ASCII PER LINE
//AND STORES THE RESULT IN THE GLOBAL VARIABLE
void *calc_highest(void *myID)
{
    //ESTABLISH ALL THE VARIABLES
    int i, j, err;
    int int_temp_array[ARRAY_SIZE];
    int nchars; //stores how many chars per wiki-line
    FILE *fd;
    char *line = (char*) malloc( LINE_SIZE + 1 );
    int startPos, endPos;
    int highest_ascii;

    //OPENS THE WIKI FILE
    fd = fopen( "/homes/ronnyim12345/Proj4/625/wiki_dump.txt", "r" );

    //ESTABLISHES THE START AND END POS
    startPos = ((int) myID) * (ARRAY_SIZE / NUM_THREADS);
	endPos = startPos + (ARRAY_SIZE / NUM_THREADS);
	
	//PRINTS THE START AND END POS
	printf("myID = %d startPos = %d endPos = %d \n", (int) myID, startPos, endPos);
    
    //SETS ALL VALUES TO ZERO
    for ( i = 0; i < ARRAY_SIZE; i++ ) {
		int_temp_array[i] = 0;
	}
    
    //GETS THE "LINE READING" TO THE RIGHT STARTING POINT
    for ( i = 0; i < startPos; i++) {
	    err = fscanf( fd, "%[^\n]\n", line);
        if( err == EOF ) break;
        nchars = strlen( line );
    }
    
    //READS IN THE LINES ONE AT A TIME, CALCULATES THE HIGHEST ASCII,
	//AND STORES THE ANSWERS IN THE TEMPORARY VARIABLE ARRAY
    for ( i = startPos; i < endPos; i++) {
        err = fscanf( fd, "%[^\n]\n", line);
        if( err == EOF ) break;
        nchars = strlen( line );
        
        highest_ascii = 0;
   
        for ( j = 0; j < nchars; j++ ) {
  
            if(line[j] > highest_ascii) {
                highest_ascii = ((int) line[j]);
            }
  
        }
        
        int_temp_array[i] += highest_ascii;
   
    }
    
    //CLOSES THE FILE
    fclose( fd );
    
    //SETS MUTEX LOCK, TRANSFERS TO GLOBAL VARIABLE, UNLOCKS
    pthread_mutex_lock (&mutexsum);
    for ( i = startPos; i < endPos; i++) {
 		int_array[i] += int_temp_array[i];
	}
    pthread_mutex_unlock (&mutexsum);
    
    //EXITS PTHREAD
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
int main() {
    
    //SETS UP VARIABLES
    int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	
	
	//INITIALIZE AND SET THREAD DETACHED ATTRIBUTE
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	
	//INITIALIZES ARRAYS
	init_arrays();
	
	//PERFORMS PTHREADS
	for (i = 0; i < NUM_THREADS; i++ ) {
	      rc = pthread_create(&threads[i], &attr, calc_highest, (void*)i);
	      if (rc) {
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	      }
	}
	

    
    //FREE ATTRIBUTE AND WAIT FOR THE OTHER THREADS
	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
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


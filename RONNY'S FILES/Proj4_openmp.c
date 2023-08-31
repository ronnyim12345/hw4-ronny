#include <omp.h>
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


//CALCULATES THE HIGHEST ASCII PER LINE
//AND STORES THE RESULT IN THE GLOBAL VARIABLE
void *calc_highest(int myID)
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

    //PERFORMS PARALLEL PROGRAMMING
	#pragma omp private(myID,i,j,err,nchars,fd,line,startPos,endPos,highest_ascii)
	{
	    
	    //ESTABLISHES THE START AND END POS
	    startPos = myID * (ARRAY_SIZE / NUM_THREADS);
		endPos = startPos + (ARRAY_SIZE / NUM_THREADS);
		
		//PRINTS THE START AND END POS
		printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);
	    
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
	    
	    //PERFORMS THE CRITICAL SECTION
	    //(TRANSFERS THE ANSWERS TO THE GLOBAL VARIABLE)
	    #pragma omp critical
	    {
	        for ( i = startPos; i < endPos; i++) {
		 		int_array[i] += int_temp_array[i];
			}
	    }
		
		
	}
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

    //SET NUMBER OF THREADS
	omp_set_num_threads(NUM_THREADS);
	
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


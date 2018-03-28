#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mpi.h"
#include "functions.h"

int main (int argc, char **argv) {

  MPI_Init(&argc,&argv);
  
  MPI_Status status;

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  //seed value for the randomizer 
  double seed = clock()+rank; //this will make your program run differently everytime
  //double seed = rank; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  //begin with rank 0 getting user's input
  unsigned int n;
  
  //declare storage for an ElGamal cryptosytem
  unsigned int p, g, h, x;

  unsigned int setup[3];

  /* Q3.1 Make rank 0 setup the ELGamal system and
    broadcast the public key information */
  if (rank == 0) {  
    printf("Enter a number of bits: "); fflush(stdout);
    char status = scanf("%u",&n);
  
    //make sure the input makes sense
    if ((n<3)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
        printf("Unsupported bit size.\n");
        return 0;   
    }
    printf("\n");

    //setup an ElGamal cryptosystem
    setupElGamal(n,&p,&g,&h,&x); 
    
    setup[0] = p;
    setup[1] = h;
    setup[2] = g;

    /* Share the setup information to all active ranks */
    int i; // avoids error on NewRiver
    for (i = 1; i < size; i++) {
        MPI_Send(setup, //data being sent
            3, //number of items
            MPI_UNSIGNED, //dtype
            i, //destination
            i, //tag
            MPI_COMM_WORLD);
    }
  }
 
  MPI_Barrier(MPI_COMM_WORLD); //wait for all ranks to reach this point

  int j;
  for (j = 1; j < size; j++) {
     if (rank == j) {
	MPI_Recv(setup,
             3, //number of items receiving 
             MPI_UNSIGNED, //type
             0, //source
             j, //tag
             MPI_COMM_WORLD, 
             &status);

       p = setup[0];
       h = setup[1];
       g = setup[2];
     }
     
  }

  MPI_Barrier(MPI_COMM_WORLD); //wait for all ranks to reach this point

  //Suppose we don't know the secret key. Use all the ranks to try and find it in parallel
  if (rank==0)
    printf("Using %d processes to find the secret key...\n", size);

  /*Q3.2 We want to loop through values i=0 .. p-2
     determine start and end values so this loop is 
     distributed amounst the MPI ranks  */
 
  unsigned int N = p-1; //total loop size
  unsigned int start, end;
  
  start = rank*(N/size); 
  end = start + (N/size) + (N % size);

  MPI_Barrier(MPI_COMM_WORLD);

  double startTime = MPI_Wtime();  
 
  //loop through the values from 'start' to 'end'
  for (unsigned int i=start;i<end;i++) {
    if (modExp(g,i+1,p)==h) {
      printf("Secret key found! x = %u \n\n",i+1);
      break;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  double endTime = MPI_Wtime();
  double time = endTime - startTime;
  double throughput = N/time; //(number of ints examined)/time

  if (rank == 0) {
  	printf("Run time for this program was %f seconds. \n", time);
  	printf("The throughput for this program was %f ops/second. \n",
	    throughput);
	printf("-----------------------------------------------------\n");
  }

  MPI_Finalize();

  return 0;
}

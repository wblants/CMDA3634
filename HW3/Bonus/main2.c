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
  
  int Niter = atoi(argv[1]);  

  if (rank == 0) {
    n = 5; //set n for this problem
    printf("Using %d MPI processes with n = 5 and Niter = %d.\n\n",size, Niter);
  }
  /*
  if (rank == 0) {
    n = atoi(argv[1]);
    printf("n = %u\n",n);

    //make sure the input makes sense
    if ((n<3)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
        printf("Unsupported bit size.\n");
        return 0;   
    }
  }*/
  
  //declare storage for an ElGamal cryptosytem
  unsigned int p, g, h, x;

  unsigned int setup[3];

  //setup an ElGamal cryptosystem
  if (rank == 0) {
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
  unsigned int i;
  unsigned int key;

  for (i = start; i < end; i++) {
    int iter;
    if (rank == 0) {
        iter = i % 1;
    }
    else {
        iter = i % start;
    }
    if (iter == Niter) {
        MPI_Recv(&key,
             1, //number of items receiving 
             MPI_UNSIGNED, //type
             MPI_ANY_SOURCE, //source
             0, //tag
             MPI_COMM_WORLD, 
             &status);
        if (modExp(g,key+1,p)==h) break;        
    }

    if (modExp(g,i+1,p)==h) {
      printf("Secret key found! x = %u \n\n",i+1);
      int j;
      for (j = 0; j < size; j++) {
        if (j == rank) {
          continue;
        }
        key = i;
        MPI_Send(&key, //data being sent
            1, //number of items
            MPI_UNSIGNED, //dtype
            j, //destination
            0, //tag
            MPI_COMM_WORLD);
      }
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

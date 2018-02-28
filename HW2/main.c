#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "functions.h"

int main (int argc, char **argv) {

	//seed value for the randomizer 
  double seed;
  
  seed = clock(); //this will make your program run differently everytime
  //seed = 0; //uncomment this and you program will behave the same everytime it's run
  
  srand48(seed);


  //begin by getting user's input
	unsigned int n;

  printf("Enter a number of bits: ");
  scanf("%u",&n);

  //make sure the input makes sense
  if ((n<2)||(n>30)) {
  	printf("Unsupported bit size.\n");
		return 0;  	
  }

  int p;
  
  /* Q2.2: Use isProbablyPrime and randomXbitInt to find a random n-bit prime number */
    int rand = randXbitInt(n);
    //Continue looking for primes if we think
    //the random int is not prime
    while (isProbablyPrime(rand) == 0) {
        rand = randXbitInt(n);
    }
    p = rand;
      
  printf("p = %u is probably prime.\n", p);

  /* Q3.2: Use isProbablyPrime and randomXbitInt to find a new random n-bit prime number 
     which satisfies p=2*q+1 where q is also prime */
    
    unsigned int q = (p-1)/2;
    while (isProbablyPrime(p) == 0 || isProbablyPrime(q) == 0) {
        p = randXbitInt(n);
        q = (p-1)/2;
        printf("-------------------\n");
        printf("p = %d & q = %d\n", p, q); 
    }

	printf("p = %u is probably prime and equals 2*q + 1. q= %u and is also probably prime.\n", p, q);  

	/* Q3.3: use the fact that p=2*q+1 to quickly find a generator */
	unsigned int g = findGenerator(p);

	printf("g = %u is a generator of Z_%u \n", g, p);  
    
    //--------Discrete Log Problem-----------
    int h = randXbitInt(n-1); // create a random private key.
    int x = 0;
    //Need to solve g^x = h mod p:
    for (x = 0; x < p; x++) {
        if (modExp(g,x,p) == h) { 
            break;
        }
    }
    
    printf("x = %d for %d^x mod %d = %d, in Z_%d\n", x, g, p, h, p);
  return 0;
}

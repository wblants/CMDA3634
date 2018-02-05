#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main() {

    int N;

    printf("Enter an upper bound:");
    scanf("%d", &N);
    
    // make storage for flags
    int *isPrime = (int*) malloc(N*sizeof(int));

    //initialize, i.e. set everyting true
    for (int n = 0; n < N; n++) isPrime[n] = 1;

    int sqrtN = (int) sqrt(N);
    for (int i = 2;i < sqrtN; i++) {
        if (isPrime[i]) { //if i is prime
            for (int j = i*i; j < N; j+=i) {
                isPrime[j] = 0; //not prime
            }
        }
    } 

    //count the number of primes we found
    int cnt = 0;
    for (int n = 0; n < N; n++) {
        if (isPrime[n]) {
            cnt++;
        }

    }  

    //make a list of them
    int *primes = (int*) malloc(cnt*sizeof(int));
    
    //loop once mor eand build the list
    cnt = 0;
    for (int n = 0; n < N; n++) {
     

    }


    free(isPrime);
    free(primes);
}


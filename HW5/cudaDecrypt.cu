#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"

//----Function Stubs-----
void readKeyInfo(const char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h);

void readMessage(const char* fileName, unsigned int* cipherText, unsigned int* a);

//----kernel and device functions----
__device__ int kernel_modprod(unsigned int a, unsigned int b, unsigned int p) {
    unsigned int za = a;
    unsigned int ab = 0;

    while(b > 0) {
        if (b % 2 == 1) ab = (ab + za) % p;
        za = (2 * za) % p;
        b /= 2;
    }
    return ab;
}

__device__ int kernel_modExp(unsigned int a, unsigned int b, unsigned int p) {
    unsigned int z = a;
    unsigned int aExpb = 1;

    while (b > 0) {
        if (b % 2 == 1) aExpb = kernel_modprod(aExpb, z, p);
        z = kernel_modprod(z, z, p);
        b /= 2;
    }
    return aExpb;
}

__global__ void findSecretKey(unsigned int g, unsigned int p, unsigned int h, unsigned int* d_x) {
  
  // find the secret key
  int tId = threadIdx.x;
  int bId = blockIdx.x;
  int bSize = blockDim.x;

  unsigned int x = tId + bId*bSize;
  
  if (kernel_modExp(g,x,p) == h) {
      *d_x = x;
  }

}

int main (int argc, char **argv) {

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;
  unsigned int Nthreads = atoi(argv[1]);

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
  
  readKeyInfo("bonus_public_key.txt", &n, &p, &g, &h);

  printf("Decrypt.c Read in:\nn = %u\np = %u\ng = %u\nh = %u\n",
          n, p, g, h);
  
  //block dimmensions
  dim3 B(Nthreads, 1, 1);

  //grid dimmensions
  dim3 G((p+Nthreads-1)/Nthreads, 1, 1);
 
  FILE* file;
  file = fopen("bonus_message.txt", "r");
  fscanf(file, "%u", &Nints);
  fclose(file);

  unsigned int* cipherText = 
      (unsigned int*) malloc(Nints*sizeof(unsigned int));
  
  unsigned int* a =
      (unsigned int*) malloc(Nints*sizeof(unsigned int));

  //readMessage("message.txt", cipherText, a);
  readMessage("bonus_message.txt", cipherText, a);

    double startTime = clock();
      
    unsigned int *d_x;
    cudaMalloc(&d_x, sizeof(unsigned int));

    findSecretKey <<< G,B >>> (g, p, h, d_x);
    cudaDeviceSynchronize();
    cudaMemcpy(&x, d_x, sizeof(unsigned int),cudaMemcpyDeviceToHost);
    
    printf("Host has x = %u\n", x);

    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);

  /* Q3 After finding the secret key, decrypt the message */
  
  ElGamalDecrypt(cipherText, a, Nints, p, x);

  unsigned int charsPerInt = Nints*(n-1)/8;
    
  unsigned char *plainText = 
    (unsigned char*) malloc(Nints*charsPerInt*sizeof(char));

  convertZToString(cipherText, Nints, plainText, charsPerInt);

  printf("Decrypted Message: %s\n", plainText);

  return 0;
}

void readKeyInfo(const char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h) {
    
    FILE *file;
    file = fopen(fileName, "r");

    for (int i = 0; i < 4; i++) {
        if (i == 0) fscanf(file, "%u", n);
        if (i == 1) fscanf(file, "%u", p);
        if (i == 2) fscanf(file, "%u", g);
        if (i == 3) fscanf(file, "%u", h);
    }

    fclose(file); 

}

void readMessage(const char* fileName, unsigned int* m, unsigned int* a) {

    FILE *file;
    file = fopen(fileName, "r"); //open the file with writing privaledges
  
    unsigned int Nints;
    fscanf(file, "%u", &Nints);

    for (int i = 0; i < Nints; i++) {
        fscanf(file, "%u", &m[i]);
        fscanf(file, "%u", &a[i]); 
    }

    //close the file when done writing
    fclose(file);

}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.h"


void readKeyInfo(char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h);
void readMessage(char* fileName, unsigned int* cipherText, unsigned int* a);

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
        if (b % 2 == 1) aExp = kernel_modprod(aExpb, z, p);
        z = modprod(z, z, p);
        b /= 2;
    }
    return aExpb;
}

__global__ int findSecretKey(unsigned int g, unsigned int p, unsigned int h,  unsigned int* h_x) {
  
  // find the secret key
  int tId = threadIdx.x;
  int bId = blockIdx.x;
  int bSize = blockDim.x;

  unsigned int x = tId + bId*bSize;

  //recreate modProd and ModExp
  if (kernel_modExp(g,x,p) == h) {
    cudaMemcpy(x, h_x, sizeof(unsigned int),cudaMemcpyDeviceToHost);
  }

  //create mem for x to copy from device to host
}

int main (int argc, char **argv) {

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
  
  readKeyInfo("public_key.txt", &n, &p, &g, &h);

  printf("Decrypt.c Read in:\nn = %u\np = %u\ng = %u\nh = %u\n",
          n, p, g, h);
    
  //copy values to device
  cudaMemcpy(n,d_n,sizeof(unsigned int),cudaMemcpyHostToDevice);
  cudaMemcpy(p,d_p,sizeof(unsigned int),cudaMemcpyHostToDevice);
  cudaMemcpy(g,d_g,sizeof(unsigned int),cudaMemcpyHostToDevice);
  cudaMemcpy(h,d_h,sizeof(unsigned int),cudaMemcpyHostToDevice);

  FILE* file;
  file = fopen("message.txt", "r");
  fscanf(file, "%u", &Nints);
  fclose(file);

  unsigned int* cipherText; 
  cudaMalloc(&cipherText, Nints*sizeof(unsigned int));
  
  cudaMemcpy(cipherText, h_cipherText, Nints*sizeof(unsigned int), cudaMemcpyDeviceToHost);

  unsigned int* a;
  cudaMalloc(&a, Nints*sizeof(unsigned int));

  cudaMemcpy(a, h_a, Nints*sizeof(unsigned int), cudaMemcpyDeviceToHost);
  
  readMessage("message.txt", cipherText, a);

  for (int i = 0; i < Nints; i++) {
    printf("cT[%d] = %u\n", i, cipherText[i]);
  }

    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }

  /* Q3 After finding the secret key, decrypt the message */
  
  unsigned int x;
  findSecretKey << x >> (g, p, h);
  cudaDeviceSynchronize();

  ElGamalDecrypt(cipherText, a, Nints, p, x);

  char *plainText;
    
  unsigned int Nchars = Nints*(n-1)/8;

  convertZToString(cipherText, Nints, plainText, Nchars);

  printf("Decrypted Message: %s\n", plainText);

  cudaFree(cipherText);
  cudaFree(a);

  return 0;
}

void readKeyInfo(char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h) {
    
    FILE *file;
    file = fopen(fileName, "r");

    char *buff;
     
    for (int i = 0; i < 4; i++) {
        if (i == 0) fscanf(file, "%u", n);
        if (i == 1) fscanf(file, "%u", p);
        if (i == 2) fscanf(file, "%u", g);
        if (i == 3) fscanf(file, "%u", h);
    }

    fclose(file); 

}

void readMessage(char* fileName, unsigned int* m, unsigned int* a) {

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

nt findSecretKey() {
  
  // find the secret key
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }

}

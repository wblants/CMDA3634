#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"

void readKeyInfo(char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h);
void readMessage(char* fileName, unsigned int* cipherText, unsigned int* a);

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

  FILE* file;
  file = fopen("message.txt", "r");
  fscanf(file, "%u", &Nints);
  fclose(file);

  unsigned int* cipherText = 
      (unsigned int *) malloc(Nints*sizeof(unsigned int));
  
  unsigned int* a =
      (unsigned int *) malloc(Nints*sizeof(unsigned int));

  printf("Got to 43\n");

  readMessage("message.txt", cipherText, a);

  printf("Got to 47\n");

  for (int i = 0; i < Nints; i++) {
    printf("cT[%d] = %u\n", i, cipherText[i]);
  }

  double startTime = clock();
  // find the secret key
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }
  }
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);

  /* Q3 After finding the secret key, decrypt the message */
  
  ElGamalDecrypt(cipherText, a, Nints, p, x);

  char *plainText;
    
  unsigned int Nchars = Nints*(n-1)/8;

  convertZToString(cipherText, Nints, plainText, Nchars);

  printf("Decrypted Message: %s\n", plainText);


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

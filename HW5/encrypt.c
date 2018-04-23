#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "functions.h"

void readFile(char*, unsigned int*, unsigned int*, unsigned int*, unsigned int*);
void writeToFile(char*, unsigned int*, unsigned int*, unsigned int);

int main (int argc, char **argv) {

	//seed value for the randomizer 
  double seed = clock(); //this will make your program run differently everytime
  //double seed = 0; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  int bufferSize = 1024;
  unsigned char *message = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

  printf("Enter a message to encrypt: ");
  int stat = scanf (" %[^\n]%*c", message); //reads in a full line from terminal, including spaces

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h;

  printf("Reading file.\n");

  /* Q2 Complete this function. Read in the public key data from public_key.txt,
    convert the string to elements of Z_p, encrypt them, and write the cyphertexts to 
    message.txt */
    readFile("public_key.txt", &n, &p, &g, &h);
    
    printf("The main got the values: \n");
    printf("n = %u\n", n);
    printf("p = %u\n", p);
    printf("g = %u\n", g);
    printf("h = %u\n", h);
   
    printf("Message: %s\n", message); 

    unsigned int charsPerInt = (n-1)/8;

    unsigned int Nchars = strlen(message);
    printf("Nchars = %u\n", Nchars);
    unsigned int Nints = strlen(message)/charsPerInt;
    printf("Nints = %u\n", Nints);

    padString(message, Nchars/Nints);

    printf("Message after pad: %s\n", message);

    //storage for message as elements of Z_p
    unsigned int *Zmessage = 
        (unsigned int *) malloc(Nints*sizeof(unsigned int)); 
  
    //storage for extra encryption coefficient 
    unsigned int *a = 
        (unsigned int *) malloc(Nints*sizeof(unsigned int)); 

    convertStringToZ(message, Nchars, Zmessage, Nints);
    
    for (int i = 0; i < Nints; i++) {
        printf("m[%d] = %d\n", i, message[i]);
    }

    ElGamalEncrypt(Zmessage, a, Nints, p, g, h);
    
    for (int i = 0; i < Nints; i++) {
        printf("Encrypted[%d] = %d\n", i, Zmessage[i]);
    } 
    
    //Write to File:
    writeToFile("message.txt",Zmessage, a, Nints);

  return 0;
}

void readFile(char* fileName, unsigned int* n, unsigned int* p, unsigned int* g, unsigned int* h) {
    
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

void writeToFile(char* filename, unsigned int* m, unsigned int* a, unsigned int Nints) {
    
    FILE *file;
    file = fopen(filename, "w+"); //open the file with writing privaledges
   
    fprintf(file, "%u\n", Nints); 
    for (int i = 0; i < Nints; i++) {
        fprintf(file, "%u %u\n", m[i], a[i]); 
    }

    //close the file when done writing
    fclose(file);
}


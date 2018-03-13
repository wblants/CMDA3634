#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef int bool;
#define true 1;
#define false 0;

//----------- Function Stubs --------------
int modProd(int, int, int);
int modExp(int, int, int);
bool isProbablyPrime(int);

void main() {
    
    
    int a,b,p;

    printf("Enter the first number a: ");
    scanf("%d", &a);
    
    printf("Enter the second number b: ");
    scanf("%d", &b);
    
    printf("Enter the group size p: ");
    scanf("%d", &p); 
    
    //modProd(a, b, p);
    // modExp(a, b, p);
    
    int N;
    printf("Enter the number N: ");
    scanf("%d", &N);

    bool daTruth = isProbablyPrime(N);
    printf("%d is probably prime?: ", N);
    if (daTruth == 1) {
        printf("True\n");
    }
    else {
        printf("False\n");
    }
    
}

//----------- Function Definitions --------------
int modProd(int a, int b, int p) {
    
    if (p == 0) {
        printf("Group size p must be > 0");
        return -1;
    }

    if (a == 0 || b == 0) {
        return 0;
    } 

    //Convert b to binary form:
    //number of bits:
    int size = (int) ( log((double) b)/log(2) ) + 1;
    
    int binary[size];
    int bCopy = b;

    for (int i = size - 1; bCopy > 0; i--) {
        int rem = bCopy % 2;
        if (rem == 1) { 
            binary[i] = 1;
        }
        else {
            binary[i] = 0;
        }
        bCopy = bCopy / 2;
    }
    
    printf("------ModProd Alg-------\n");

    printf("b = %d in binary is: ", b);
    for (int i = 0; i < size; i++) {
        printf("%d", binary[i]);
    }
    printf("\n");
    
    //Modular Product Algorithm

    int ab = 0;
    int za = a; 
    
    for (int i = size-1; i >= 0; i--) {
        if (binary[i] == 1) ab = (ab + za*(binary[i])) % p;
        za = (2*za) % p;
    }

    printf("%d*%d mod %d is: %d\n", a, b, p, ab);
    
    return ab;
}

int modExp(int a, int b, int p) {
    
    //Convert b to binary form:
    //number of bits:
    int size = (int) ( log((double) b)/log(2) ) + 1;
    
    int binary[size];
    int bCopy = b;

    for (int i = size - 1; bCopy > 0; i--) {
        int rem = bCopy % 2;
        if (rem == 1) { 
            binary[i] = 1;
        }
        else {
            binary[i] = 0;
        }
        bCopy = bCopy / 2;
    }
    printf("-------ModExP Alg--------\n");
     
    printf("b = %d in binary is: ", b);
    for (int i = 0; i < size; i++) {
        printf("%d", binary[i]);
    }

    printf("\n");
    
    //Modular Exponentiation Algorithm
    int z = a;
    int aExpb = 1;

    //Start with the rightmost bit
    for (int i = size-1; i >= 0; i--) {
        if (binary[i] == 1) {
            aExpb = modProd(aExpb, z, p);
        }
        z = modProd(z, z, p);
    }
    
    printf("%d^%d mod %d = %d\n", a, b, p, aExpb);
    
    return aExpb;
}


bool isProbablyPrime(int N) {
    
    int smallPrimeList[] = {2, 3, 5, 7, 11, 13, 17, 23, 29, 31};
    
    int d = 1;
    double r = log((N-1)/d)/log(2);
    //While r is not an integer:
    while (r != (int) r) {
        d += 2;
        r = log((N-1)/d)/log(2);
    }
    printf("r = %d, d = %d\n", (int) r, d);
    
    int x;
    int a = 2;
    for (int i=0; i < (int) sizeof(smallPrimeList)/sizeof(int); i++) {
        x = modExp(a, d, N);
        if (x == 1 || x == N-1) {
            a++;
            continue;
        }

        for (int i=1; i < r; i++) {
            x = modProd(x,x,N);
            if (x == 1) return false;
            if (x == N-1) {
                a++;
                break; //continue to next k 
            }
        }
        //catch the nested if statement:
        if (x == N-1) continue;
        return false; 
    }
    return true;
}






#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int gcd(int, int);
int intPow(int, int);

void main() {
   
    int p;
    int isGen = 1;

    printf("What is the group size? (Must be a prime number): ");
    scanf("%d", &p);
    
    int r;
    
    //Find r by checking for coprimes
    for (int i = 2; i < p; i++) {
        int GCD = gcd(i, p);
        if (GCD == 1) { //if coprime
            r = i;
            break;
        }
    }
    
    int g = 2;    
    //Check g^r =/= 1
    int g2TheR = intPow(g, r) % p;
    while (g2TheR == 1) {
        g++;
        g2TheR = intPow(g, r) % p;
    }

    printf("%d is a generator of %d\n", g, p);
}


int intPow(int a, int b) {

    int result;

    for (int i = 0; i < b; i++) {
        result = result * a;    
    }

    return result;
}


int gcd(int a, int b) {
    
    int temp = a;
    //Swap the values if a < b
    if (a < b) {
        a = b;
        b = temp;
    }

    int remainder = 1;
    int count = 0;

    while (a % b != 0) {
        remainder = a % b;
        a = b;
        b = remainder;
        count++;
    }

    if (count == 0 && a / b == b) {
        remainder = a / b;
    }
    else {
        remainder = b;
    }
    
    return remainder;
}

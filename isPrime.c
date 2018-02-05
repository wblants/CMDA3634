#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main() {
    
    int a;
    int isPrime = 1; //true
    
    printf("Enter the number: ");
    scanf("%d", &a);
    
    if (a <= 1) isPrime = 0; //false

    //Using trial division:
    int n = 2;
    while (n < a) {
        if (a % n == 0) {
            isPrime = 0; //false
            break;
        }
        n++;
    } 

    if (isPrime == 1) {
        printf("%d is prime.\n", a);
    }
    else {
        printf("%d is not prime.\n", a);
    } 
}

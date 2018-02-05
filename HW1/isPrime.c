#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//There are no real concerns for this algorithm. 
//Signed negative ints are handled as well as
//ints 1-3. 
void main() {
    
    int a;
    int isPrime = 1; //true
    
    //User input:
    printf("Enter the number: ");
    scanf("%d", &a);

    //(-inf, 1] are not prime
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
    
    //If true:
    if (isPrime == 1) {
        printf("%d is prime.\n", a);
    }
    else {
        printf("%d is not prime.\n", a);
    } 
}

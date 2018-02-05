#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//This algrotihm should be relatively resistant to overflow
//errors casused by operations on large integers. There may
//be issues with signed integers (particuarlly negative).
void main() {
    
    int a; 
    int b;
    
    //Prompt user for input:
    printf("Enter the first number: ");
    scanf("%d", &a);
    printf("Enter the second number: ");
    scanf("%d", &b);
    
    int temp = a;
    //Swap the values if a < b
    if (a < b) {
        a = b;
        b = temp;
    }
    //Euclidian algorithm:
    int remainder = 1;
    int count = 0;

    while (a % b != 0) {
        remainder = a % b;
        a = b;
        b = remainder;
        count++;
    }
    
    //Conditions for a % b = 0 (pairs that do not enter the loop):
    if (count == 0 && a / b == b) {
        remainder = a / b;
    }
    else {
        remainder = b;
    }

    printf("The greatest common denominator is: %d\n", remainder); 

}

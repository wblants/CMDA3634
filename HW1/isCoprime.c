#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Modified gcd algorithm:
int gcd(int a, int b); 

//Signed ints will cause issues with correctness
//of the result. The code will compile and run but
//may not compute the correct answer.
void main() {

    int a, b;
    printf("Enter the first number: ");
    scanf("%d", &a);
    printf("Enter the second number: ");
    scanf("%d", &b);

    int GCD = gcd(a, b);
    
    if (GCD == 1) {
        printf("%d and %d are coprime\n", a, b);
    }
    else {    
        printf("%d and %d are not coprime\n", a, b);
    }
}

//Modified GCD algorithm:
int gcd(int x, int y) {
    
    int a, b;
    a = x;
    b = y;
    
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

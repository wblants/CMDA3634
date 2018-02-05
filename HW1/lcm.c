#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//this version of gcd returns the result as an integer
//rather than printing the answer to the screen.
int gcd(int x, int y);

//LCM algorithm starts here:
//This algorithm is susceptible to overflow errors to very large
//integers (see line 23-25). The algorithm will still compile and
//run for overflow errors, but the result will be wrong. 
void main() {
   
    int a, b;
    
    //prompt for user input:
    printf("Enter the first number: ");
    scanf("%d", &a);
    printf("Enter the second number: ");
    scanf("%d", &b);
    //compute the gcd of a,b 
    int GCD = gcd(a, b);
    //For very large numbers (greater than 30 bits), there may be issues
    //multiplying here
    int lcm = a * b / GCD;

    printf("The least common multiple is: %d\n", lcm);  

}

//Modified GCD algorithm:
//signed ints may cause issues with correctness.
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

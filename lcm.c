#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int gcd(int x, int y);

void main() {
   
    int a, b;
    
    printf("Enter the first number: ");
    scanf("%d", &a);
    printf("Enter the second number: ");
    scanf("%d", &b);

    int GCD = gcd(a, b);
    //For very large numbers (greater than 30 bits), there may be issues
    //multiplying here
    int lcm = a * b / GCD;

    printf("The least common multiple is: %d\n", lcm);  

}

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

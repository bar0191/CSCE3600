/*
 ============================================================================
 Name        : minor1.c
 Author      : Brandon A. Reid
 Version     : v1
 Copyright   : 2016
 Description : This program will ask the user to enter any character from the
 keyboard and covert the ASCII values to Decimal, hexidecimal, octal, and 
 binary
 ============================================================================
 */
#include <stdio.h>

// function prototypes
void binaryCnvrt(int valueToCnvrt);

//main
int main(void)
{
    // variable declarations
    char userInput;
    int charToInt;
    
    // prompt user for ASCII Char, getchar()
    printf("Enter an ASCII character: ");
    userInput = getchar();
    
    // print value entered by user, and convert to int
    printf("The ASCII value of %c is: \n", userInput);
    charToInt = (int) userInput;
    
    // print C converted values
    printf("     dec -- %d \n", userInput);     // dec
    printf("     hex -- %x \n", userInput);     // hex
    printf("     oct -- %o \n", userInput);     // oct
    
    // print binary conversion
    printf("     bin -- ");
    binaryCnvrt(charToInt);
    printf("\n");
    
    return 0;
}

/*
 ============================================================================
 Function    : binaryCnvrt(int)
 Parameters  : int valueToCnvrt
 Return      : nothing
 Description : Cnvrt decimal int value to binary with simple division and
 modulus operation (using recursion)
 
 recursively divides the ASCII value until it reaches zero, and also uses the
 modulus operator to gain the remainder
 
divide a int down by 2, and taking the remainder
 ============================================================================
 */

void binaryCnvrt(int valueToCnvrt)
{
    // if the ASCII value is greater than zero
    if (valueToCnvrt > 0)
    {
        // recursively divide by 2
        binaryCnvrt(valueToCnvrt/2);
    }
    
    // keep getting the modulus(remainder) of each divided value
    printf("%d", valueToCnvrt % 2);
}
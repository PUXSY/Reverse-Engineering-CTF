/*
CTF_Level5
 This program hides the correct flag as a binary string and checks your input against it.
 - decimalToBinary() converts each character’s ASCII value into an 8-bit binary string.
 - textToBinary() converts your whole input into space-separated binary bytes.
 - validateFlag() XORs your binary input with the stored binaryFlag; if every bit matches, the XOR sum stays 0.
 - main() reads your input, strips the newline, and prints Congratulations! only if the binary comparison is a perfect match.
`https://cyberchef.org/#recipe=To_Binary('Space',8)&input=Q1RGe0FTQ0lJLWlzLW1PcmUtbGlrZV9CSU5BU0NJSSEhIX0` (HOW TO SOLVE: Convert the binary string to text to get the flag)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *binaryFlag = "01000011 01010100 01000110 01111011 01000001 01010011 01000011 01001001 01001001 00101101 01101101 01001111 01110010 01100101 00101101 01101100 01101001 01101011 01100101 01011111 01000010 01001001 01001110 01000001 01010011 01000011 01001001 01001001 00100001 00100001 00100001 01111101";

void decimalToBinary(int decimal, char *binary) {
    for (int i = 0; i < 8; i++) {
        binary[i] = '0';
    }
    binary[8] = '\0';
    

    int i = 7;
    while (decimal > 0 && i >= 0) {
        binary[i--] = (decimal % 2) + '0';
        decimal /= 2;
    }
}

void textToBinary(char *text, char *binary) {
    char *octet = malloc(9);
    if(octet == NULL)
        exit(1);
    
    int binary_pos = 0;
    int binaryLength = (strlen(text) * 9); 
    while(*text && binary_pos < binaryLength - 9) {
        decimalToBinary(*text, octet);
        
        for(int i = 0; i < 8; i++) {
            binary[binary_pos++] = octet[i];
        }
        binary[binary_pos++] = ' ';
        ++text;
    }
    
    binary[binary_pos] = '\0';
    free(octet);
}

int validateFlag(char *userInput) {
    int binaryLength = (strlen(userInput) * 9); 
    char *userInputBinary = malloc(binaryLength + 1);
    
    if(userInputBinary == NULL)
        exit(1);

    textToBinary(userInput, userInputBinary);
    
    int xorResult = 0;
    int i = 0, j = 0;
    
    while(binaryFlag[i] != '\0' && userInputBinary[j] != '\0') {
        if(binaryFlag[i] == ' ') {
            i++;
            continue;
        }
        if(userInputBinary[j] == ' ') {
            j++;
            continue;
        }
        
        int digit1 = binaryFlag[i] - '0';
        int digit2 = userInputBinary[j] - '0';
        xorResult += (digit1 ^ digit2);
        
        i++;
        j++;
    }
    free(userInputBinary);
    return xorResult;
}

int main() {
    char userInput[64];
    
    printf("===== CTF Level 5 Challenge =====\n");
    printf("Find and enter the correct flag:\n> ");
    
    fgets(userInput, sizeof(userInput), stdin);
    
    size_t len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
    }

    if (validateFlag(userInput) == 0) {
        printf("\nCongratulations! You found the correct flag!\n");
    } else {
        printf("\nWrong!\n");
    }

    return 0;
}

// CTF_Level4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *binaryFlag = "01000011 01010100 01000110 01011111 00110100 01011111 01111011 00110001 00111000 00110100 00111001 00110101 00110011 00110110 00110100 00110011 00110011 00110100";

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
    
    // Calculate XOR of binary strings
    int xorResult = 0;
    int i = 0, j = 0;
    
    // Skip spaces and XOR only the binary digits
    while(binaryFlag[i] != '\0' && userInputBinary[j] != '\0') {
        if(binaryFlag[i] == ' ') {
            i++;
            continue;
        }
        if(userInputBinary[j] == ' ') {
            j++;
            continue;
        }
        
        // XOR the binary digits (converting '0'/'1' chars to 0/1 integers)
        int digit1 = binaryFlag[i] - '0';
        int digit2 = userInputBinary[j] - '0';
        xorResult += (digit1 ^ digit2);
        
        i++;
        j++;
    }
    free(userInputBinary);
    return xorResult; // Return 0 if strings match perfectly (XOR of same bits is 0)
}

int main() {
    char userInput[64];
    
    printf("===== CTF Level 4 Challenge =====\n");
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

/* 
   When decoded, the flag is CTF_4_{18495364334}
   The encoding uses a simple XOR with a position-dependent key
   To solve this challenge, you need to:
   1. Identify the encoding mechanism
   2. Reverse the XOR operation with the correct key pattern
*/
/*
CTF_Level6 
 This program checks a flag by hashing your input and comparing it to a fixed number `4910934987744585413` Corect flag hash.
 The hash function used is djb2: start with 0x5381 and for each character do: hash = hash * 33 + char.
 To change the correct flag, run the program, enter your new flag, copy the printed hash,
 and replace the value inside actualValidator() with that hash.
 To solve the challenge, find a string whose djb2 hash equals the hardcoded value.
 All random logic, timing checks, and extra validators are decoys and can be ignored.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const unsigned long long Correct_hash = 12763515080480605377;


unsigned long long hash(char *str) {
    unsigned long long hash = 0x5381;
    int c;
    
    while ((c = *str++))
        //  =  ((0x5381 << 5) + 0x5381) + c
        hash = ((hash << 5) + hash) + c;
    
    return hash;
}


typedef int (*validator_func)(char*);

int actualValidator(char *input) {
    const unsigned long long correct_hash = 16377509176557920611;
    return hash(input) == correct_hash;
}


int decoyValidator(char *input) {
    return 0;
}
void printHash(char *str) {
    printf("Hash of \"%s\" is: %llu\n", str, hash(str));
}

int main() {
    char UserInput[64];
    validator_func validate;
    srand(time(NULL) % 100);
    
    printf("===== CTF Level 6 Challenge =====\n");
    printf("This challenge requires deeper analysis.\n");
    printf("Enter the flag to proceed:\n> ");
    
    fgets(UserInput, sizeof(UserInput), stdin);
    
    size_t len = strlen(UserInput);
    if (len > 0 && UserInput[len - 1] == '\n') {
        UserInput[len - 1] = '\0';
    }
    clock_t start = clock();
   

    int selector = (int[]){0, 2, 3}[rand() % 3];
    if (selector == 0 || selector == 2 || selector == 3) {
        validate = actualValidator;
    } else {
        validate = decoyValidator;
    }
    

    if ((strlen(UserInput) > 3) && ((UserInput[0] + UserInput[1]) % 256 != 0)) {
        validate = actualValidator;
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    if (time_spent > 0.1) {
        printf("Processing error. Please try again.\n");
        return 1;
    }
    
    printHash(UserInput);
    if (validate(UserInput)) {
        printf("\nCorrect!\n");
    } else {
        printf("\nIncorrect. Try again.\n");
    }
    
    return 0;
}


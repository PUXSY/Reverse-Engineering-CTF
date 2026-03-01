/*
CTF_Level4
 builds the correct flag dynamically, then checks your input against it.
 - Flag() allocates memory and starts the flag with "CTF_".
 - FlagP() appends "4_", and Flag2P() appends the final part "{84536134584571335}".
 - getFlag() chains those functions to construct the full flag.
 - CheckFlag() compares what you typed to the generated flag using strcmp.
 - main() reads your input, trims the newline, and prints Correct! if it matches, otherwise Wrong!.

*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* Flag();
char* FlagP(char *flag);
char* Flag2P(char *flag);
char* getFlag();

int CheckFlag(char *UserInput) {
    int result = strcmp(UserInput, getFlag()) == 0;
    return result;
}



char* getFlag() {
    char *flag = Flag();
    flag = FlagP(flag); 
    flag = Flag2P(flag);
    return flag;        
}

char* Flag() {
    char *s = malloc(64);
    if (s != NULL) {
        strcpy(s, "CT");
    }
    return s;
}

char* FlagP(char *flag) {
    strcat(flag, "F{This_iS-gett");
    return flag;
}

char* Flag2P(char *flag) {
    strcat(flag, "Ing-difficu|t.}");
    return flag;
}

int main() {
    char UserInput[64];

    printf("===== CTF Level 4 Challenge =====\n");
    printf("Enter the flag:\n");
    fgets(UserInput, sizeof(UserInput), stdin);
    
    size_t len = strlen(UserInput);
    if (len > 0 && UserInput[len - 1] == '\n') {
        UserInput[len - 1] = '\0';
    }

    if (CheckFlag(UserInput)) {
        printf("Correct!\n");
    } else {
        printf("Wrong!\n");
    }

    return 0;
}

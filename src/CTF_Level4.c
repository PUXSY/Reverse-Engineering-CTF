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
        strcpy(s, "CTF_");
    }
    return s;
}

char* FlagP(char *flag) {
    strcat(flag, "4_");
    return flag;
}

char* Flag2P(char *flag) {
    strcat(flag, "{84536134584571335}");
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

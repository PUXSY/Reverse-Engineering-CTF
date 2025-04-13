// CTF_Level5 (Fixed Overflow)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Custom hashing function
unsigned long long hash(char *str) {
    unsigned long long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    
    return hash;
}

// Function pointer type for validation
typedef int (*validator_func)(char*);

// Validator that will be selected at runtime
int actualValidator(char *input) {
    // The correct hash value for "CTF_5_{53615844327}"
    const unsigned long long correct_hash = 11763518080480605357;
    
    // We compare hashes instead of the actual string
    return hash(input) == correct_hash;
}

// Decoy validator that always returns false
int decoyValidator(char *input) {
    // This function exists to confuse static analysis
    return 0;
}

// Debug function to check hash - can be removed in production
void printHash(char *str) {
    printf("Hash of \"%s\" is: %llu\n", str, hash(str));
}

int main() {
    char UserInput[64];
    validator_func validate;
    
    // Initialize random seed
    srand(time(NULL) % 100);
    
    printf("===== CTF Level 5 Challenge =====\n");
    printf("This challenge requires deeper analysis.\n");
    printf("Enter the flag to proceed:\n> ");
    
    fgets(UserInput, sizeof(UserInput), stdin);
    
    size_t len = strlen(UserInput);
    if (len > 0 && UserInput[len - 1] == '\n') {
        UserInput[len - 1] = '\0';
    }
    
    // Anti-debugging: Check elapsed time to detect single-stepping
    clock_t start = clock();
    
    // Looks like random selection but actually always picks the real validator
    // This is to confuse static analysis
    int selector = rand() % 4;
    if (selector == 0 || selector == 2 || selector == 3) {
        validate = actualValidator;
    } else {
        validate = decoyValidator;
    }
    
    // More misdirection with apparent conditions that always resolve the same way
    if ((strlen(UserInput) > 3) && ((UserInput[0] + UserInput[1]) % 256 != 0)) {
        validate = actualValidator;
    }
    
    clock_t end = clock();
    
    // If too much time has elapsed, it might be a debugger
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    if (time_spent > 0.1) {
        printf("Processing error. Please try again.\n");
        return 1;
    }
    
    printHash(UserInput);
    if (validate(UserInput)) {
        printf("\nExcellent! You've cracked level 5!\n");
    } else {
        printf("\nIncorrect. Try again.\n");
    }
    
    return 0;
}

/*
   The correct flag is CTF_5_{53615844327}
   
   This challenge requires:
   1. Understanding the validation is done via hash comparison
   2. Realizing the validator selection logic always selects actualValidator
   3. Either brute-forcing, reverse engineering the hash, or finding the hash 
      collision for the correct flag
*/
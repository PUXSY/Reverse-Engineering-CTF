// CTF_Level2

#include <stdio.h>
#include <string.h>

int CheckFlag(char *UserInput) {
    char flag[21] = "CTF_2_{672288856595}";
    if (strcmp(UserInput, flag) == 0) {
        return 1; // Correct
    } else {
        return 0; // Wrong
    }
}

int main() {
  char UserInput[20];

  printf("===== CTF Level 2 Challenge =====\n");
  printf("Enter the flag:\n");
  fgets(UserInput, sizeof(UserInput), stdin);
  
  size_t len = strlen(UserInput);
  if (len > 0 && UserInput[len - 1] == '\n') {
      UserInput[len - 1] = '\0';
  }

  if (CheckFlag(UserInput) == 1) {
      printf("Correct!\n");
      return 0;
  } else {
      printf("Wrong!\n");
      return 0;
  }

  printf("Something went wrong!\n");
  printf("Please try again.\n");
  return 0;
}

/*
The flag is CTF_2_{672288856595}
the flag is stored in the CheckFlag function.
os go to main and see the CheckFlag function go to it and see the flag.
*/

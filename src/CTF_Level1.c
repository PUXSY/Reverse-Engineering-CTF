// CTF_Level1

#include <stdio.h>
#include <string.h>

int main() {
  char flag[17] = "CTF_1_{266152184}";
  char UserInput[20];

  printf("Enter the flag:\n");
  fgets(UserInput, sizeof(UserInput), stdin);
  
  size_t len = strlen(UserInput);
  if (len > 0 && UserInput[len - 1] == '\n') {
      UserInput[len - 1] = '\0';
  }

  if (strcmp(UserInput, flag) == 0) {
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

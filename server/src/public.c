#include "../include/head.h"
int split(char *buf, char *command[])
{
  if (buf == NULL)
  {
    return -1;
  }
  command[0] = strtok(buf, " ");
  printf("You entered: %s\n", command[0]);
  for (int i = 0; command[i] != NULL; i++)
  {
    command[i + 1] = strtok(NULL, " ");
    if (command[i + 1] != NULL)
    {
      printf("The argument %d is: %s\n", i + 1, command[i + 1]);
    }
  }
  return 0;
}
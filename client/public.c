#include "head.h"
int split(char *buf, char **command)
{
  if (buf == NULL)
  {
    return -1;
  }
  command[0] = strtok(buf, " ");
  printf("You entered: %s\n", command[0]);
  for (int i = 1; command[i] != NULL; i++)
  {
    command[i] = strtok(NULL, " ");
    printf("The argument %d is: %s\n", i, command[i]);
  }
  return 0;
}
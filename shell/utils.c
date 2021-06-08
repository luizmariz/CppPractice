#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char* trim_str(char* str) {
  char* end;

  while (isspace((unsigned char)*str)) str++;

  if (*str == 0) return str;

  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "shell.h"

void print_greeting() {
  printf("******************************************");
  printf("\n\nMAB353 - LAB 4 (Custom Shell)");
  printf("\n\nLuiz Gustavo Oliveira Mariz – DRE 118033995");
  printf("\nRodrigo Araújo – DRE 118045366");
  printf("\n\n******************************************");
  printf("\n\n");
}

void print_prompt1() {
  char cwd[1024];

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd() error");
  }

  char* username = getenv("USER");
  fprintf(stderr, "~%s %s@mabshell> ", cwd, username);
}

void print_prompt2() {
  fprintf(stderr, "> ");
}
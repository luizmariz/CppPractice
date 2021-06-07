#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

enum BuiltinCommands {
  NOT_BUILTIN = 0,
};

int main(int argc, char** argv) {
  char* cmd;

  init_shell();

  do {
    print_prompt1();
    cmd = read_cmd();
    Command command = parse_cmd(cmd);

    if (!cmd) {
      exit(EXIT_SUCCESS);
    }

    if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0) {
      free(cmd);
      continue;
    }

    if (strcmp(cmd, "exit\n") == 0) {
      free(cmd);
      break;
    }

    if (is_builtin_cmd(command.cmd) == NOT_BUILTIN) {
		  int childPid = fork();
      if (childPid < 0) {
        perror("error: failed to fork not builtin command");
      } else if (childPid == 0) {
        execute_cmd(command);
      } else {
        int status;
		 		if (waitpid(childPid, &status, 0) < 0) {
          perror("error: waitpid");
        }
      }
		}

    free(cmd);
    free(command.args);
    free(command.cmd);

  } while (1);

  exit(EXIT_SUCCESS);
}

void init_shell() {
  print_greeting();
}

char* read_cmd(void) {
  char buf[1024];
  char* ptr = NULL;
  char ptrlen = 0;

  while (fgets(buf, 1024, stdin)) {
    int buflen = strlen(buf);

    if (!ptr) {
      ptr = malloc(buflen + 1);
    } else {
      char* ptr2 = realloc(ptr, ptrlen + buflen + 1);

      if (ptr2) {
        ptr = ptr2;
      }
      else {
        free(ptr);
        ptr = NULL;
      }
    }

    if (!ptr) {
      fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
      return NULL;
    }

    strcpy(ptr + ptrlen, buf);

    if (buf[buflen - 1] == '\n') {
      if (buflen == 1 || buf[buflen - 2] != '\\') {
        return ptr;
      }

      ptr[ptrlen + buflen - 2] = '\0';
      buflen -= 2;
      print_prompt2();
    }

    ptrlen += buflen;
  }

  return ptr;
}

Command parse_cmd(char* str) {
  Command command;

  int i, args_count = -1; // Token offset
  for (i = 0; str[i] != '\0'; i++) {
    if ((str[i] != ' ' && str[i+1] == ' ')||(str[i+1] == '\0' && str[i] != ' ')) {
      args_count++;
    }
  }

  char* token = strtok(str, " ");

  command.cmd = malloc(sizeof(char*));
  command.args = malloc(args_count*sizeof(char*));
  command.args_count = args_count;

  strcpy(command.cmd, token);

  i = 0;
  while (i < args_count) {
    command.args[i] = trim_str(strtok(NULL, " "));
    i++;
  }

  return command;
}

int is_builtin_cmd(char* cmd) {
  return NOT_BUILTIN;
}

void execute_cmd(Command command) {
  // Docs says:
  //  The first argument, by convention, should point
  //  to the filename associated with the file being executed.  The
  //  array of pointers must be terminated by a null pointer
  char** args = malloc((command.args_count+2)*sizeof(char*));

  args[0] = command.cmd;

  int i;
  for (i = 1; i < command.args_count+1; i++) {
    args[i] = command.args[i-1];
  }

  args[command.args_count+1] = NULL;

  int status = execv(args[0], args);

  free(args);

  if (status < 0) {
    perror("error: failed to execv() command");
    exit(1);
  }
}
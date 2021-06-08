#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include "shell.h"

pid_t cur_fg_job;
bool has_fg = false;

int main(int argc, char** argv) {
  char* cmd;

  signal(SIGINT, handle_sigint);
  signal(SIGTSTP, handle_sigstop);

  // signal(SIGCHLD, handle_sigchld);
  // Não atendia, segundo as docs https://man7.org/linux/man-pages/man2/sigaction.2.html:
  // sigaction com a flag SA_SIGINFO me permite acessar o pid do processo.
  // Além disso, sigaction não é tão primitivo quando signal permitindo uma "queue" de sinais
  struct sigaction sa;

  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = handle_sigchld;

  sigaction(SIGCHLD, &sa, NULL);

  init_shell();

  do {
    print_prompt1();

    cmd = read_cmd();


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

    Command command = parse_cmd(cmd);

    int status;
    int builtin_command;
    int childPid;
    if ((builtin_command = is_builtin_cmd(command.cmd))) {
      switch (builtin_command) {
        case CD:
          handle_cd(command);
          break;
        case JOBS:
          handle_jobs();
          break;
        case FG:
          handle_job_action(command, FG);
          break;
        case BG:
          handle_job_action(command, BG);
          break;
      }
		} else {
		  childPid = fork();
      if (childPid < 0) {
        perror("error: failed to fork not builtin command");
        exit(1);
      }

      if (childPid > 0) {
        insert_job_in_joblist(childPid, RUNING, command);

        if (command.args[command.args_count - 1][0] != '&') {
          cur_fg_job = childPid;
          has_fg = true;
        }
      }

      if (childPid == 0) {
        execute_cmd(command);
      }
    }

    while (has_fg) {
      if (waitpid(cur_fg_job, &status, WUNTRACED | WCONTINUED) < 0) {
        perror("error: waitpid");
        exit(1);
      }

      if (WIFEXITED(status)) {
        remove_job_from_joblist(cur_fg_job);
      }

      has_fg = false;
    }

    free(cmd);
    free(command.args);
    free(command.cmd);

  } while (1);

  free_joblist();
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
    if (buf != NULL) {
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
  if (strncmp(cmd, "cd", strlen("cd")) == 0) {
    return CD;
  }

  if (strncmp(cmd, "jobs", strlen("jobs")) == 0) {
    return JOBS;
  }

  if (strncmp(cmd, "fg", strlen("fg")) == 0) {
    return FG;
  }

  if (strncmp(cmd, "bg", strlen("bg")) == 0) {
    return BG;
  }

  return NOT_BUILTIN;
}

void execute_cmd(Command command) {
  int total_args = command.args_count;

  if (command.args_count > 0 && command.args[command.args_count - 1][0] == '&') {
    total_args = command.args_count - 1;
  }

  // Docs says:
  //  The first argument, by convention, should point
  //  to the filename associated with the file being executed.  The
  //  array of pointers must be terminated by a null pointer
  char** args = malloc((total_args+2)*sizeof(char*));

  args[0] = command.cmd;

  int i;
  for (i = 1; i < total_args+1; i++) {
    args[i] = command.args[i-1];
  }

  args[total_args+1] = NULL;

  int status = execv(args[0], args);

  if (status < 0) {
    perror("error: failed to execv() command");
    exit(1);
  }

  free(args);
}

void job_to_bg(int pid) {
  kill(pid, SIGCONT);
  update_job_status(pid, RUNING);
  print_job(pid);
}

void job_to_fg(int pid) {
  kill(pid, SIGCONT);
  update_job_status(pid, RUNING);
  cur_fg_job = pid;
  has_fg = true;
  print_job(pid);
}

void handle_job_action(Command command, BuiltinCommand builtin_command) {
  Job* job;

  if (command.args_count == 0) {
    Job* job = get_last_job();
    job_to_fg(job->pid);
    return;
  }

  if (command.args[0][0] == '%') {
    int id = atoi(command.args[0] + 1);
    job = find_job_by_id(id);
  } else {
    int pid = atoi(command.args[0]);
    job = find_job_by_pid(pid);
  }

  if (!job) {
    perror("error: job not found");
    return;
  }

  if (job->status == EXITED) {
    perror("error: job already exited");
    remove_job_from_joblist(job->pid);
    return;
  }

  if (builtin_command == FG) job_to_fg(job->pid);
  if (builtin_command == BG) job_to_bg(job->pid);
}

void handle_cd(Command command) {
  int status;

  if (command.args_count == 0) {
    status = chdir(getenv("HOME"));
    return;
  } else if (command.args_count > 1) {
    printf("error: more args than expected in cd");
    return;
  }

  status = chdir(command.args[0]);
  if (status < 0) {
    perror("error: failed to execute cd");
  }
}

void handle_jobs() {
  print_joblist();
}

void handle_sigint() {
  if (has_fg) {
    kill(cur_fg_job, SIGINT); // Envia sigint to fg
    has_fg = false;
    puts("");
  }
}

void handle_sigstop() {
  if (has_fg) {
    kill(cur_fg_job, SIGSTOP); // Envia sigstop to fg
    has_fg = false;
    puts("");
  }
}

void handle_sigchld(int sig, siginfo_t* info, void* ucontext) {
  int status;
  // Com a opção WNOHANG setada, se o child ainda não estiver morto
  // retorna 0. Caso contrario retorna o pid e armazena o status na variável passada
  if (waitpid(info->si_pid, &status, WNOHANG) == 0) {
    update_job_status(info->si_pid, SUSPENDED);
    return;
  }

  // Checa se emitiu um sinal e se esse sinal foi um sinal de stop
  if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSTOP) {
    update_job_status(info->si_pid, SUSPENDED);
    return;
  }

  // Checa se o processo terminou
  if (WIFEXITED(status)) {
    update_job_status(info->si_pid, EXITED);
    return;
  }
}
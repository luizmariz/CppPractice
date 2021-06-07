#ifndef SHELL_H
#define SHELL_H

typedef struct {
  char* cmd;
  char** args;
  int args_count;
} Command;

void print_greeting();
void print_prompt1();
void print_prompt2();

char* read_cmd();
Command parse_cmd(char*);
void execute_cmd(Command);
int is_builtin_cmd(char* cmd);
void init_shell();

char* trim_str(char*);

#endif
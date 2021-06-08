#ifndef SHELL_H
#define SHELL_H

typedef struct {
  char* cmd;
  char** args;
  int args_count;
} Command;

typedef enum {
  NOT_BUILTIN = 0,
  FG,
  BG,
  JOBS,
  CD
} BuiltinCommand;

typedef enum JobStatus {
  RUNING = 0,
  SUSPENDEND,
  EXITED
} JobStatus;

typedef struct {
  int pid;
  int id;
  Command command;
  JobStatus status;
} Job;

typedef struct JobNode {
  Job job;
  struct JobNode* next;
} JobNode;

// Prompt
void print_greeting();
void print_prompt1();
void print_prompt2();

// Main
void init_shell();
void execute_cmd(Command);
void handle_cd(Command);
void handle_jobs();
void handle_sigint();
void handle_sigstop();
void handle_sigchld();
void handle_job_action(Command, BuiltinCommand);
int is_builtin_cmd(char*);
char* read_cmd();
Command parse_cmd(char*);

// Utils
char* trim_str(char*);

// Jobs
void print_joblist();
void free_joblist();
void remove_job_from_joblist(int);
void update_job_status(int, JobStatus);
int insert_job_in_joblist(int, JobStatus, Command);
int is_joblist_empty();
int joblist_length();
Job* get_last_job();
Job* find_job_by_pid(int);
Job* find_job_by_id(int);

#endif
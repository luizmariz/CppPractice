#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"

JobNode* head = NULL;
JobNode* last = NULL;

void print_joblist() {
  JobNode* ptr = head;
  while (ptr != NULL) {
    printf("\n[%d]  %s  pid %d  %s", ptr->job.id, get_status_name(ptr->job.status), ptr->job.pid, ptr->job.cmd);
    ptr = ptr->next;
  }
  printf("\n\n");
}

void print_job(int pid) {
  Job* job = find_job_by_pid(pid);
  printf("\n[%d]  %s  pid %d  %s\n", job->id, get_status_name(job->status), job->pid, job->cmd);
}

int insert_job_in_joblist(int pid, JobStatus status, Command command) {
  int id = 1;

  if (last != NULL) {
    id = last->job.id + 1;
  }

  char* cmd = malloc(strlen(command.cmd)+1);
  strcpy(cmd, command.cmd);

  int i;
  for (i = 0; i < command.args_count; i++) {
    strcat(cmd, " ");
    strcat(cmd, command.args[i]);
  }

  Job job = {
    .pid = pid,
    .id = id,
    .cmd = cmd,
    .status = status
  };

  JobNode* new_node = (JobNode*) malloc(sizeof(JobNode));

  new_node->job = job;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
    last = new_node;
    return id;
  }

  last->next = new_node;
  last = new_node;

  return id;
}

int is_joblist_empty() {
  return head == NULL;
}

void free_joblist() {
  JobNode* ptr = head;

  while (ptr != NULL) {
    JobNode* node = ptr;
    ptr = ptr->next;
    kill(node->job.pid, SIGTERM);
    free(node);
  }

  head = NULL;
  last = NULL;
}

int joblist_length() {
  int length = 0;
  JobNode* ptr;

  for (ptr = head; ptr != NULL; ptr = ptr->next) {
    length++;
  }

  return length;
}

Job* find_job_by_pid(int pid) {
  JobNode* cur = head;

  if (head == NULL) {
    return NULL;
  }

  while (cur->job.pid != pid) {
    if (cur->next == NULL) {
      return NULL;
    } else {
      cur = cur->next;
    }
  }

  return &(cur->job);
}

Job* find_job_by_id(int id) {
  JobNode* cur = head;

  if (head == NULL) {
    return NULL;
  }

  while (cur->job.id != id) {
    if (cur->next == NULL) {
      return NULL;
    } else {
      cur = cur->next;
    }
  }

  return &(cur->job);
}

void remove_job_from_joblist(int pid) {
  JobNode* cur = head;
  JobNode* prev = NULL;

  if (head == NULL) {
    return;
  }

  while (cur->job.pid != pid) {
    if (cur->next == NULL) {
      return;
    } else {
      prev = cur;
      cur = cur->next;
    }
  }

  if (cur == head) {
    head = head->next;
  } else {
    prev->next = cur->next;
  }

  free(cur);
}

void update_job_status(int pid, JobStatus status) {
  Job* job = find_job_by_pid(pid);
  job->status = status;
}

Job* get_last_job() {
  return &(last->job);
}
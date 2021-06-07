#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int n = 0;
int pid;
void conta(int sig)
{
  n++;
  sleep(1);
  kill(pid, SIGCONT);
  return;
}
int main()
{
  int i;
  signal(SIGUSR1, conta);
  pid = fork();
  if (pid == 0)
  {
    for (i = 0; i < 5; i++)
    {
      kill(getppid(), SIGUSR1);
      printf("enviado SIGUSR1 ao pai \n");
      kill(getpid(), SIGSTOP);
    }
    exit(0);
  }
  wait(NULL);
  printf("n=%d\n", n);
  exit(0);
}
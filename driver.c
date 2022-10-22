#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

char *newargv[] = { NULL, argv[2], argv[2], argv[3], NULL };
char *newenviron[] = { NULL };

const char *name = "SHM";
const int SIZE = 4096;

int shm_fd;
char *ptr;

if (argc <= 2 || atoi(argv[2]) < 2) {
    fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int childNum = atoi(argv[2]);
  char* consumerName = argv[1];

  newargv[0] = argv[1];

  pid_t pid;

  printf("Parent: Playing Chinese whisper game with %d processes\n",childNum);

  for (int i = 0; i < childNum; i++)
  {
    wait(NULL);
      pid = fork();
      if (pid == 0 && i == 0)
      {
          sprintf(newargv[1], "%d", i);
          execve(consumerName, newargv, newenviron);
          kill(getpid(), SIGTERM);
      }
      else if (pid == 0 && i > 0 && i + 1 != childNum)
      {
        sprintf(newargv[1], "%d",i);
        execve(consumerName, newargv, newenviron);
        kill(getpid(), SIGTERM);
      }
      else if (pid == 0 && i + 1 == childNum)
      {
        sprintf(newargv[1], "%d", i* (-1));
        execve(consumerName, newargv, newenviron);
        kill(getpid(), SIGTERM);
      }
      else if (pid > 0)
      {
        
      }
  }
      wait(NULL);
      kill(getpid(),SIGTERM);

  return 0;
}
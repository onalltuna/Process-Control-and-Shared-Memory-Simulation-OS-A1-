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


char *newargv[] = { NULL, argv[2], argv[3], NULL };
char *newenviron[] = { NULL };

const char *name = "SHM";
const int SIZE = 4096;

int shm_fd;
char *ptr;

if (argc <= 2) {
    fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int childNum = atoi(argv[2]);
  char* consumerName = argv[1];

  newargv[0] = argv[1];
  
  pid_t pid;

  for (int i = 0; i < childNum; i++)
  {
      pid = fork();
      if (pid == 0 && i == 0)
      {
          newargv[1] = "0";
        //   printf("First child\n");
          execve(consumerName, newargv, newenviron);
          kill(getpid(), SIGTERM);
      }
      else if (pid > 0)
      {
          wait(NULL);

          shm_fd = shm_open(name, O_RDONLY, 0666);
          if (shm_fd == -1)
          {
              printf("shared memory failed\n");
              exit(-1);
          }

          ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
          if (ptr == MAP_FAILED)
          {
              printf("Map failed\n");
              exit(-1);
          }

          printf("%s\n", ptr);
          kill(getpid(), SIGTERM);
      }
  }

  return 0;
}
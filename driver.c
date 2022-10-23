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

int main(int argc, char *argv[])
{

  // newargv will the arguments for the consumer-producer program
  char *newargv[] = {NULL, argv[2], argv[2], argv[3], NULL};
  char *newenviron[] = {NULL};

  // name of the shared memory will be SHM and size will be 4096
  const char *name = "SHM";
  const int SIZE = 4096;

  int shm_fd;
  char *ptr;

  // check if the number of command line argments at least 3 and number of childs is at least 2
  if (argc <= 2 || atoi(argv[2]) < 2)
  {
    fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // number of childs(number of players of the chinese whisper game) is the 3rd command line argumant
  // and the name of the consumer-producer program is the second command line argument
  int childNum = atoi(argv[2]);
  char *consumerName = argv[1];

  newargv[0] = argv[1];

  pid_t pid;

  printf("Parent: Playing Chinese whisper game with %d processes\n", childNum);

  // for loop iterated for the number of childs
  for (int i = 0; i < childNum; i++)
  {
    // wait until there is no other child process is running
    wait(NULL);
    pid = fork();
    // case of the first child
    if (pid == 0 && i == 0)
    {

      sprintf(newargv[1], "%d", i);
      execve(consumerName, newargv, newenviron);
      kill(getpid(), SIGTERM);
    }
    // case for the children which are not first and not last
    else if (pid == 0 && i > 0 && i + 1 != childNum)
    {
      // consumer-producer program will be called
      // with the parameter i(ith child)
      sprintf(newargv[1], "%d", i);
      execve(consumerName, newargv, newenviron);
      kill(getpid(), SIGTERM);
    }
    // case for the last child
    else if (pid == 0 && i + 1 == childNum)
    {
      // consumer-producer program will be called
      // with the parameter -i(last child)
      sprintf(newargv[1], "%d", i * (-1));
      execve(consumerName, newargv, newenviron);
      kill(getpid(), SIGTERM);
    }
    else if (pid > 0)
    {
    }
  }
  // wait until all other processes are completed and terminate the parent process
  wait(NULL);
  kill(getpid(), SIGTERM);

  return 0;
}
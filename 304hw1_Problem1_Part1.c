#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


int main() 
{
    pid_t pid;
    struct timeval current_time;
    pid = fork();

    if (pid == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("\nPID of child = %d\n", getpid());
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            printf("seconds : %ld\n", current_time.tv_sec);
            sleep(1);
        }
    }
    else if (pid > 0)
    {
        sleep(5);
        kill(pid, SIGTERM);
        printf("Child <%d> killed \n", pid);
        kill(getpid(),SIGTERM);
    }

    return 0;
}

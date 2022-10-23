#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

int main()
{
    // pid_t data type to hold pid values
    pid_t pid;
    struct timeval current_time;
    // fork for creating the child
    pid = fork();

    // if pid is 0 then this is the child process
    if (pid == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            // child prints its own pid and current time then sleeps 1 seconds
            // in this way child prints current time and its pid every second
            printf("\nPID of child = %d\n", getpid());
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            printf("seconds : %ld\n", current_time.tv_sec);
            sleep(1);
        }
    }
    // if pid is greater than 0 then this is the parent process
    else if (pid > 0)
    {
        // parent process sleeps for 5 seconds then kills child process,prints its output and then terminates
        sleep(5);
        kill(pid, SIGTERM);
        printf("Child <%d> killed \n", pid);
        kill(getpid(), SIGTERM);
    }

    return 0;
}

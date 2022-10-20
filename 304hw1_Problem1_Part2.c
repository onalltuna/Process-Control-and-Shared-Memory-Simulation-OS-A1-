#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

int main()
{
    pid_t pids[4];
    for (int i = 0; i < 4; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            for (int k = 0; k < 5; k++)
            {
                struct timeval current_time;
                gettimeofday(&current_time, NULL);
                printf("PID of child = %d  seconds : %ld\n", getpid(), current_time.tv_sec);
                sleep(1);
            }
            kill(getpid(), SIGTERM);
        }
    }
    sleep(5);
    for (int i = 0; i < 4; i++)
    {
        kill(pids[i], SIGTERM);
        printf("Child <%d> is killed\n", pids[i]);
    }

    kill(getpid(), SIGTERM);

    return 0;
}

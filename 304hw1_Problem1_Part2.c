#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

int main()
{
    // list of pid_t to store the pids of child processes
    pid_t pids[4];

    // for loop executes 4 time to create 4 child processes
    for (int i = 0; i < 4; i++)
    {
        // store the ith childs pid in pids list
        pids[i] = fork();

        // if pid equals zero then this is one of the child processes
        if (pids[i] == 0)
        {
            // similar to the part1 each child prints its own pid and current time
            // once a second for 5 seconds
            for (int k = 0; k < 5; k++)
            {
                struct timeval current_time;
                gettimeofday(&current_time, NULL);
                printf("PID of child = %d  seconds : %ld\n", getpid(), current_time.tv_sec);
                sleep(1);
            }
            // kill extra grandchildren othervise there would be 2^4 processes
            kill(getpid(), SIGTERM);
        }
    }
    // parent process waits for 5 seconds and kills all the child processes and prints kill message
    sleep(5);
    for (int i = 0; i < 4; i++)
    {
        kill(pids[i], SIGTERM);
        printf("Child <%d> is killed\n", pids[i]);
    }
    // finally parent process terminates
    kill(getpid(), SIGTERM);

    return 0;
}

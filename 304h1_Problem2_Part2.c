#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    // pid_t list to store pids of child processes
    pid_t pids[2];

    // write number will be written by the parent process
    // read numbers will be read by each child process
    int writeNumber;
    int readNumber1;
    int readNumber2;

    // outout1 and output2 will be the results that parent will use
    // after child processes modify the readnumbers
    int output1;
    int output2;

    // 3 pipes are used 1 from parent to both children
    // 1 from first child to parent and 1 from second child to parent
    int fd[3][2];

    // parent gets the standart input from the user
    printf("Please enter the number: \n");
    scanf("%d", &writeNumber);

    // for checking if pipes are succesfully initialized
    for (int i = 0; i < 3; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            fprintf(stderr, "Pipe failed");
            return 1;
        }
    }

    // for loop iterates 2 times for both children
    for (int i = 0; i < 2; i++)
    {
        // parent process writes to the pipe
        write(fd[0][1], &writeNumber, sizeof(writeNumber));

        // child creation with fork
        pids[i] = fork();
        if (pids[i] > 0)
        {
        }
        // case for the first child
        else if (pids[i] == 0 && i == 0)
        {
            // close the writing end of first pipe and read from the first pipe
            close(fd[0][1]);
            read(fd[0][0], &readNumber1, sizeof(readNumber1));
            printf("First Child: Input %d , Output = PID %d * %d\n", readNumber1, getpid(), readNumber1);
            // modify the input, store it in output1 and send it with the second pipe
            int outputt1 = getpid() * readNumber1;
            sleep(20);
            write(fd[1][1], &outputt1, sizeof(outputt1));
            kill(getpid(), SIGTERM);
        }
        // case for the second child
        else if (pids[i] == 0 && i == 1)
        {
            // close the writing end of first pipe and read from the first pipe
            close(fd[0][1]);
            read(fd[0][0], &readNumber2, sizeof(readNumber2));
            printf("Second Child: Input %d , Output = PID %d / %d \n", readNumber2, getpid(), readNumber2);
            sleep(20);
            // modify the input, store it in output2 and send it with the third pipe
            int output2 = (int)(getpid() / readNumber2);
            write(fd[2][1], &output2, sizeof(output2));
            kill(getpid(), SIGTERM);
        }
    }
    // case for the parent
    if (getpid() > 0)
    {

        // read from second and third pipe and print the sum of the inputs read from the pipes
        close(fd[1][1]);
        close(fd[2][1]);
        read(fd[1][0], &output1, sizeof(output1));
        close(fd[1][0]);
        read(fd[2][0], &output2, sizeof(output2));
        close(fd[2][0]);
        printf("Final result is = %d\n", output1 + output2);
    }
    // parent kills both children and terminates
    kill(pids[0], SIGTERM);
    kill(pids[1], SIGTERM);
    kill(getpid(), SIGTERM);

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define READ_END	0
#define WRITE_END	1



int main()
{

    pid_t pids[2];

    int writeNumber;
    
    int readNumber1;
    int readNumber2;

    int output1;
    int output2;

    
    int fd[3][2];

    printf("Please enter the number: \n");
    scanf("%d", &writeNumber);
   
    for (int i = 0; i < 3; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            fprintf(stderr, "Pipe failed");
            return 1;
        }
    }
    

    for (int i = 0; i < 2; i++)
    {
        
        write(fd[0][1], &writeNumber, sizeof(writeNumber));
        
        pids[i] = fork();

        if (pids[i] > 0) {
            
        }
        else if (pids[i] == 0 && i == 0)
        {
            close(fd[0][1]);
            read(fd[0][0], &readNumber1, sizeof(readNumber1));
            printf("First Child: Input %d , Output = PID %d * %d\n", readNumber1, getpid(),  readNumber1);
            int outputt1 = getpid() * readNumber1;
            // printf("outputttt: %d\n",outputt1);
            // // close(fd[READ_END]); 
            write(fd[1][1], &outputt1,sizeof(outputt1));
            // // close(fd[WRITE_END]);
            kill(getpid(), SIGTERM);

        } else if (pids[i] == 0 && i == 1) 

        {
          
            close(fd[0][1]);
            read(fd[0][0], &readNumber2, sizeof(readNumber2));
            printf("Second Child: Input %d , Output = PID %d / %d \n", readNumber2, getpid(), readNumber2);
            int output2 = (int)(getpid() / readNumber2);
            // close(fd[READ_END]);
            // int output2 = getpid() * number2;
            write(fd[2][1], &output2,sizeof(output2));
            kill(getpid(), SIGTERM);

        } 

    }

    if(getpid() > 0) {
        // close(fd[WRITE_END]);
        read(fd[1][0],&output1,sizeof(output1));
        close(fd[1][0]);
        read(fd[2][0],&output2,sizeof(output2));
        close(fd[2][0]);
        printf("Final result is = %d\n" , output1 + output2);
        
    }
    
    kill(pids[0],SIGTERM);
    kill(pids[1],SIGTERM);
    kill(getpid(),SIGTERM);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h> 



int main(int argc, char *argv[]) {

    // printf("%d\n",argc);
    // printf("%s\n",argv[0]);
    // printf("%s\n",argv[1]);
    // printf("%s\n",argv[2]);
    // printf("%s\n",argv[3]);

    const int SIZE = 4096;
    int shm_fd;
    const char *name = "SHM";
    char *ptr;
    char *ptr2;

    int ran1,ran2;
    srand (getpid());


    if (strcmp(argv[1],"0") == 0) {

        printf("Child %d : %s\n",atoi(argv[1]) + 1, argv[3]);

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        sprintf(ptr, "%s", argv[3]);
        ptr += strlen(argv[3]);
    }
    else if (atoi(argv[1]) > 0 )
    {
        // printf("argv1 : %d---argv2 : %d\n",atoi(argv[1]),atoi(argv[2]));
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        ptr2 = ptr;
        ran1 = rand() % strlen(ptr2) + 1;
        ran2 = rand() % strlen(ptr2) + 1;
        char x = ptr2[ran2];
        ptr2[ran2] = ptr2[ran1];
        ptr2[ran1] = x;

        strcpy(ptr, ptr2);

        printf("Child %d : %s\n", atoi(argv[1]) + 1, ptr);
    }
    else if ((atoi(argv[1]) < 0))
    {
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        ptr2 = ptr;
        ran1 = rand() % strlen(ptr2) + 1;
        ran2 = rand() % strlen(ptr2) + 1;
        char x = ptr2[ran2];
        ptr2[ran2] = ptr2[ran1];
        ptr2[ran1] = x;

        strcpy(ptr, ptr2);

        printf("Child %d : %s\n", -1 * atoi(argv[1]) + 1, ptr);

        if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
    }
    
    return 0;
}

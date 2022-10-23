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

    //name and the size of the shared memory
    const char *name = "SHM";
    const int SIZE = 4096;
    
    int shm_fd;
    char *ptr;
    char *ptr2;

    //ran1 and ran2 are the random values for swaping the characters of the input string
    //seed with the pid for each child process
    int ran1,ran2;
    srand (getpid());

    //if input from driver program is 0 this is the first child
    if (strcmp(argv[1],"0") == 0) {

        //print the starting string of the game
        printf("Child %d : %s\n",atoi(argv[1]) + 1, argv[3]);
        //open a shared memory read/write
        //adjust the size of the shared memory
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        //map the character pointer to the shared memory
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        //check if mapping is succesful
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        //change the value of the string in the shared memory to starting string
        sprintf(ptr, "%s", argv[3]);
        ptr += strlen(argv[3]);
    }
    //case for the children which are not first or last
    else if (atoi(argv[1]) > 0 )
    {
        //open the shared memory
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        //map to the shared memory
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        //check if mapping is succesful
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        //ptr2 is another character pointer to be used to swapping characters of ptr
        ptr2 = ptr;
        //choose random values for swapping
        ran1 = rand() % strlen(ptr2) + 1;
        ran2 = rand() % strlen(ptr2) + 1;
        //swap the selected characters
        char x = ptr2[ran2];
        ptr2[ran2] = ptr2[ran1];
        ptr2[ran1] = x;

        //store the new string in the shared memory
        strcpy(ptr, ptr2);
        //print the swapped string
        printf("Child %d : %s\n", atoi(argv[1]) + 1, ptr);
    }
    //case for the last child
    else if ((atoi(argv[1]) < 0))
    {
        //open the shared memory
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        //map to the shared memory
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        //check if the mapping is succesful
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        //swap the characters of the string in the shared memory
        ptr2 = ptr;
        ran1 = rand() % strlen(ptr2) + 1;
        ran2 = rand() % strlen(ptr2) + 1;
        char x = ptr2[ran2];
        ptr2[ran2] = ptr2[ran1];
        ptr2[ran1] = x;

        strcpy(ptr, ptr2);

        //print the swapped string
        printf("Child %d : %s\n", -1 * atoi(argv[1]) + 1, ptr);

        //unlink the shared memory because this is the last child
        if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
    }
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>




int main(int argc, char *argv[]) {

    // printf("%d\n",argc);
    // printf("%s\n",argv[0]);
    // printf("%s\n",argv[1]);
    // printf("%s\n",argv[2]);

    const int SIZE = 4096;
    int shm_fd;
    const char *name = "SHM";
    void *ptr;



	// if (shm_fd == -1) {
	// 	printf("shared memory failed\n");
	// 	exit(-1);
	// }

    if (strcmp(argv[1],"0") == 0) {
        printf("First child\n");


        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);

        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        sprintf(ptr, "%s", argv[2]);
        ptr += strlen(argv[2]);
    }
    
    return 0;
}
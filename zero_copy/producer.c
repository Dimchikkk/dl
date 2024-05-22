#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define SIZE 4096
#define SHM_NAME "/my_shm"

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    char *map = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    close(fd);

    while (true) {
        const char *message = "Hello, zero-copy world!";
        strcpy(map, message);
        printf("Producer wrote: %s\n", message);
        sleep(100000); // Delay for demonstration purposes
    }

    if (munmap(map, SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}

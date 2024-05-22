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
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);

    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    char *map = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    close(fd);

    printf("Consumer read: %s\n", map);

    if (munmap(map, SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}

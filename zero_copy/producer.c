#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define FILEPATH "shared_file"
#define SIZE 4096

int main() {
    int fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
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
        sleep(10000000); // Delay for demonstration purposes
    }

    if (munmap(map, SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}

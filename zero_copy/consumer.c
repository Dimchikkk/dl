#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define FILEPATH "shared_file"
#define SIZE 4096

int main() {
    sleep(1); // Wait for producer to finish writing

    int fd = open(FILEPATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
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

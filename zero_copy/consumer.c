#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define FILEPATH "shared_file"
#define SIZE 4096
#define SEM_PRODUCER "/producer_sem"
#define SEM_CONSUMER "/consumer_sem"

int main() {
    // Open semaphores
    sem_t *producer_sem = sem_open(SEM_PRODUCER, 0);
    if (producer_sem == SEM_FAILED) {
        perror("sem_open producer");
        return 1;
    }

    sem_t *consumer_sem = sem_open(SEM_CONSUMER, 0);
    if (consumer_sem == SEM_FAILED) {
        perror("sem_open consumer");
        return 1;
    }

    // Wait for producer to finish writing
    sem_wait(producer_sem);

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

    // Signal producer that data has been read
    sem_post(consumer_sem);

    if (munmap(map, SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    // Close semaphores
    sem_close(producer_sem);
    sem_close(consumer_sem);

    return 0;
}

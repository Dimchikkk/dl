#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define FILEPATH "shared_file"
#define SIZE 4096
#define SEM_PRODUCER "/producer_sem"
#define SEM_CONSUMER "/consumer_sem"

int main() {
    // Create or open semaphores
    sem_t *producer_sem = sem_open(SEM_PRODUCER, O_CREAT | O_EXCL, 0666, 0);
    if (producer_sem == SEM_FAILED) {
        perror("sem_open producer");
        return 1;
    }

    sem_t *consumer_sem = sem_open(SEM_CONSUMER, O_CREAT | O_EXCL, 0666, 0);
    if (consumer_sem == SEM_FAILED) {
        perror("sem_open consumer");
        return 1;
    }

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

    const char *message = "Hello, zero-copy world!";
    strcpy(map, message);
    printf("Producer wrote: %s\n", message);

    // Signal consumer that data is ready
    sem_post(producer_sem);

    // Wait for consumer to finish reading
    sem_wait(consumer_sem);

    if (munmap(map, SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    // Close and unlink semaphores
    sem_close(producer_sem);
    sem_close(consumer_sem);
    sem_unlink(SEM_PRODUCER);
    sem_unlink(SEM_CONSUMER);

    return 0;
}

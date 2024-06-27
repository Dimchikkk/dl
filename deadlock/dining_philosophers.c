#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // Number of philosophers
#define TOTAL_EAT_TARGET 100000  // Target total eat count

pthread_mutex_t forks[N];  // Mutexes representing forks
int eat_count[N] = {0};    // Array to store how many times each philosopher has eaten
int total_eat_count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher(void *num);

int main() {
    pthread_t tid[N];
    int philosopher_numbers[N];

    // Initialize forks
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < N; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    // Join threads
    for (int i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    // Print each philosopher's eat count and check total eat count
    int sum = 0;
    printf("Eat counts:\n");
    for (int i = 0; i < N; i++) {
        printf("Philosopher %d ate %d times.\n", i, eat_count[i]);
        sum += eat_count[i];
    }
    printf("Total eat count: %d\n", sum);

    return 0;
}

void *philosopher(void *num) {
    int phil_num = *(int *)num;

    while (1) {
        int left = phil_num;
        int right = (phil_num + 1) % N;

        // Ensure that philosophers pick up forks in a consistent order to prevent deadlock
        if (left > right) {
            int temp = left;
            left = right;
            right = temp;
        }

        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);

        // Check total eat count under lock
        pthread_mutex_lock(&count_mutex);
        if (total_eat_count < TOTAL_EAT_TARGET) {
            // Eating
            eat_count[phil_num]++;
            total_eat_count++;
        }
        pthread_mutex_unlock(&count_mutex);

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        // Check if total eat count reaches target
        if (total_eat_count >= TOTAL_EAT_TARGET) {
            break;
        }
    }

    return NULL;
}

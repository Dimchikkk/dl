// Credit: https://youtu.be/7zI_4CKk-3Y?si=Bcwn8AOlmuIxe7Ep
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SIZE 10
static int buffer[SIZE];
static int first_empty_slot;
static int last_filled_slot;
static int count;

static pthread_mutex_t peek;

static pthread_cond_t not_full;
static pthread_cond_t not_empty;

int consume(void)
{
  int value;

  pthread_mutex_lock(&peek);
  while (count == 0)
    pthread_cond_wait(&not_empty, &peek);
  value = buffer[last_filled_slot];
  last_filled_slot = (last_filled_slot + 1) % SIZE;
  count--;
  pthread_cond_signal(&not_full);
  pthread_mutex_unlock(&peek);
  return value;
}

int produce(int value)
{
  pthread_mutex_lock(&peek);
  while (count == SIZE)
    pthread_cond_wait(&not_full, &peek);
  buffer[first_empty_slot] = value;
  first_empty_slot = (first_empty_slot + 1) % SIZE;
  count++;
  pthread_cond_signal(&not_empty);
  pthread_mutex_unlock(&peek);
  return value;
}

void *producer_thread(void *blah)
{
  produce(42);
  return NULL;
}

void *consumer_thread(void *blah)
{
  printf("%i\n", consume());
  // this is waiting for producer
  printf("%i\n", consume());
  return NULL;
}

int main()
{
  first_empty_slot = last_filled_slot = count = 0;

  pthread_mutex_init(&peek, NULL);

  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty, NULL);

  // produce one value on main thread
  produce(1);

  pthread_t consumer_placeholder;
  // consume two values in another thread, thread will wait for producer to produce new value
  pthread_create(&consumer_placeholder, NULL, consumer_thread, NULL);
  
  pthread_t producer_placeholder;
  // produce new value in another thread
  pthread_create(&producer_placeholder, NULL, producer_thread, NULL);
  
  pthread_join(consumer_placeholder, NULL);
  pthread_join(producer_placeholder, NULL);

  return 0;
}

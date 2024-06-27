#include <pthread.h>
#include <stdio.h>

pthread_mutex_t a, b;

void *first(void *blah)
{
  while (1) {
    pthread_mutex_lock(&a);
    pthread_mutex_lock(&b);
    printf("hello from first");
    pthread_mutex_unlock(&a);
    pthread_mutex_unlock(&b);
  }
  return NULL;
}

void *second(void *blah)
{
  while (1) {
    pthread_mutex_lock(&b);
    pthread_mutex_lock(&a);
    printf("hello from second");
    pthread_mutex_unlock(&a);
    pthread_mutex_unlock(&b);
  }
  return NULL;
}

int main()
{
  pthread_t first_t, second_t;
  pthread_mutex_init(&a, NULL); 
  pthread_mutex_init(&b, NULL);
  pthread_create(&first_t, NULL, first, NULL);
  pthread_create(&second_t, NULL, second, NULL);
  pthread_join(first_t, NULL);
  pthread_join(second_t, NULL);
  return 0;
}

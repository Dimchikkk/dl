#include <stdio.h>
#include <pthread.h>

unsigned next_seed = 1;

unsigned rand(void)
{
  next_seed = next_seed * 23423423423 + 23432;
  return (unsigned)(next_seed >> 16) % 32768;
}

void srand(unsigned new_seed)
{
  next_seed = new_seed;
}

void *call_rand(void *blah)
{
  rand();
  return NULL;
}

int main()
{
  pthread_t placeholder;
  srand(0);
  pthread_create(&placeholder, NULL, call_rand, NULL);
  // Comment the following line to have race condition
  pthread_join(placeholder, NULL);
  printf("%u\n", rand()); 
  printf("%u\n", rand()); 
  printf("%u\n", rand()); 
}

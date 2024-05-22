#include <stdlib.h>
#include <string.h>
#include "hello.h"

char *greeting = "Hello, Version 1";

struct hello *greet(void) {
  struct hello *p = malloc(sizeof *p);
  if (!p) {
      return NULL; // Allocation failed
  }
  p->type = TYPE_CHARP;
  p->value = strdup(greeting);
  if (!p->value) {
      free(p); // Clean up memory if strdup fails
      return NULL;
  }
  return p;
}

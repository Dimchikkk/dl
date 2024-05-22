#include <stdlib.h>
#include "hello.h"

int greeting = 42;

struct hello *greet(void) {
    struct hello *p = malloc(sizeof *p);
    if (!p) {
        return NULL; // Check for allocation failure
    }
    p->type = TYPE_INT;
    p->value = malloc(sizeof(int));
    if (!p->value) {
        free(p); // Free memory before returning NULL
        return NULL; // Check for allocation failure
    }
    *(int *)(p->value) = greeting;
    return p;
}

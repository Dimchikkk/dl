#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include "hello.h"

#define LIBHELLO_SO "libhello.so"

void load_and_call_greet() {
    // Open the shared library
    void *dynamic_lib = dlopen(LIBHELLO_SO, RTLD_LAZY);
    if (!dynamic_lib) {
        fprintf(stderr, "%s\n", dlerror());
        return;
    }

    // Define a function pointer for the function we want to call
    char *(*greet)(void);

    // Get the function from the shared library
    greet = (char *(*)(void)) dlsym(dynamic_lib, "greet");

    // Check for errors
    char *error;
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(dynamic_lib); // Close the library before exiting
        return;
    }

    // Call the function and print the result
    struct hello *result = greet();
    if (result->type == TYPE_INT) {
        printf("%d\n", *((int *)result->value));
    } else if (result->type == TYPE_CHARP) {
        printf("%s\n", result->value);
    }

    // Close the shared library
    dlclose(dynamic_lib);
}

int main() {
    while (1) {
        load_and_call_greet();
        sleep(5);  // Wait for 5 seconds before reloading the library
    }
    return 0;
}

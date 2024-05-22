#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

// Correct ARM64 machine code for mov x0, #42; ret
unsigned char func_code[] = {
    0x40, 0x05, 0x80, 0xd2, // mov x0, #0x2a (42)
    0xc0, 0x03, 0x5f, 0xd6  // ret
};

int main() {
    // Size of the function
    size_t func_size = sizeof(func_code);

    // Allocate memory
    void *mem = mmap(NULL, func_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Copy the function to the allocated memory
    memcpy(mem, func_code, func_size);

    // Mark the memory as executable
    if (mprotect(mem, func_size, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect");
        return 1;
    }

    // Define a function pointer with the same signature as the function
    int (*func)() = mem;

    // Call the function
    int result = func();
    printf("Result: %d\n", result); // Should print 42

    // Clean up
    if (munmap(mem, func_size) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}

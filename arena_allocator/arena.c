#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

// Define the MemoryArena structure
typedef struct {
    unsigned char *base; // Base address of the allocated memory
    size_t size;         // Total size of the arena
    size_t offset;       // Current offset for the next allocation
} MemoryArena;

// ObjectA definition
typedef struct {
    int value;
} ObjectA;

// ObjectB definition with dependency on ObjectA
typedef struct {
    ObjectA *dependency;
    int value;
} ObjectB;

// Function to initialize the memory arena using mmap
void init_arena(MemoryArena *arena, size_t size) {
    arena->base = (unsigned char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (arena->base == MAP_FAILED) {
        fprintf(stderr, "Failed to mmap memory for arena\n");
        exit(EXIT_FAILURE); // Exit if mmap fails
    }
    arena->size = size;    // Set the size of the arena
    arena->offset = 0;     // Initialize the offset to 0
}

// Function to allocate memory from the arena
void *arena_alloc(MemoryArena *arena, size_t size) {
    if (arena->offset + size > arena->size) { // Check if there is enough space left in the arena
        fprintf(stderr, "Arena out of memory\n");
        return NULL; // Return NULL if there is not enough space
    }
    void *ptr = arena->base + arena->offset; // Calculate the address of the allocated block
    arena->offset += size;                   // Update the offset for the next allocation
    return ptr;                              // Return the address of the allocated block
}

// Function to reset the arena, reclaiming all allocated memory without freeing the block
void reset_arena(MemoryArena *arena) {
    arena->offset = 0; // Reset the offset to 0
}

// Function to destroy the arena, freeing the allocated memory using munmap
void destroy_arena(MemoryArena *arena) {
    munmap(arena->base, arena->size); // Free the allocated memory using munmap
    arena->base = NULL;  // Set the base pointer to NULL
    arena->size = 0;     // Reset the size to 0
    arena->offset = 0;   // Reset the offset to 0
}

int main() {
    MemoryArena arena;
    size_t arena_size = 1024 * 1024; // 1MB arena size for testing
    init_arena(&arena, arena_size);

    // Allocate ObjectA within the arena
    ObjectA *a = (ObjectA *)arena_alloc(&arena, sizeof(ObjectA));
    a->value = 10;

    // Allocate ObjectB with dependency on ObjectA within the same arena
    ObjectB *b = (ObjectB *)arena_alloc(&arena, sizeof(ObjectB));
    b->dependency = a;
    b->value = 20;

    // Access ObjectB's dependency after ObjectA has been allocated
    printf("ObjectB's dependency value: %d\n", b->dependency->value);

    // Destroy the arena, freeing all allocated memory
    destroy_arena(&arena);

    return 0;
}

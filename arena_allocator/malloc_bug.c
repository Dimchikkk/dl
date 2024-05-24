#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
} ObjectA;

typedef struct {
    ObjectA *dependency;
    int value;
} ObjectB;

ObjectA *create_object_a(int value) {
    ObjectA *obj = (ObjectA *)malloc(sizeof(ObjectA));
    if (obj == NULL) {
        fprintf(stderr, "Failed to allocate memory for ObjectA\n");
        exit(EXIT_FAILURE);
    }
    obj->value = value;
    return obj;
}

ObjectB *create_object_b(ObjectA *dependency, int value) {
    ObjectB *obj = (ObjectB *)malloc(sizeof(ObjectB));
    if (obj == NULL) {
        fprintf(stderr, "Failed to allocate memory for ObjectB\n");
        exit(EXIT_FAILURE);
    }
    obj->dependency = dependency;
    obj->value = value;
    return obj;
}

void destroy_object_a(ObjectA *obj) {
    free(obj);
}

void destroy_object_b(ObjectB *obj) {
    free(obj);
}

int main() {
    // Create ObjectA
    ObjectA *a = create_object_a(10);

    // Create ObjectB with dependency on ObjectA
    ObjectB *b = create_object_b(a, 20);

    // BUG: ObjectB should be destroyed before ObjectA
    destroy_object_a(a);

    // Access ObjectB's dependency after ObjectA has been freed (potential bug)
    printf("ObjectB's dependency value: %d\n", b->dependency->value);

    // Destroy ObjectB
    destroy_object_b(b);

    return 0;
}

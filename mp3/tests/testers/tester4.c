#include "tester-utils.h"

#define START_MALLOC_SIZE (1 * G)
#define STOP_MALLOC_SIZE (1 * K)

void *reduce(void *ptr, int size) {
    // printf("reduce begins\n");
    if (size > STOP_MALLOC_SIZE) {
        // printf("here1\n");
        void *ptr1 = realloc(ptr, size / 2);
        // printf("here2\n");
        void *ptr2 = malloc(size / 2);
        // printf("here3\n");
        // printf("\n");
        if (ptr1 == NULL || ptr2 == NULL) {
            fprintf(stderr, "Memory failed to allocate!\n");
            exit(1);
        }

        ptr1 = reduce(ptr1, size / 2);
        ptr2 = reduce(ptr2, size / 2);

        if (*((int *)ptr1) != size / 2 || *((int *)ptr2) != size / 2) {
            fprintf(stderr, "Memory failed to contain correct data after many "
                            "allocations!\n");
            exit(2);
        }
        // printf("passed1\n");

        free(ptr2);
        // printf("passed2\n");
        ptr1 = realloc(ptr1, size);
        // printf("passed3\n");
        if (*((int *)ptr1) != size / 2) {
            fprintf(stderr,
                    "Memory failed to contain correct data after realloc()!\n");
            exit(3);
        }

        // printf("passed4\n");

        *((int *)ptr1) = size;
        return ptr1;
    } else {
        *((int *)ptr) = size;
        // printf("passed5\n");

        return ptr;
    }
}

int main() {
    (void) malloc(1);

    int count = 0;

    int size = START_MALLOC_SIZE;
    while (size > STOP_MALLOC_SIZE) {

        // printf("loop begins\n");

        void *ptr = malloc(size);
        ptr = reduce(ptr, size / 2);
        free(ptr);
        size /= 2;

        // count+=1;
        // printf("loop is %d\n",count);
    }

    fprintf(stderr, "Memory was allocated, used, and freed!\n");
    return 0;
}

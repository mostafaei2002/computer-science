#include <stdio.h>

void main() {
    int num_array[100];
    for (int i = 0; i < sizeof(num_array)/sizeof(num_array[0]); i++) {
        num_array[i] = i;
        printf("%d\n", i);
    }
    // Arrays are fixed in Size because memory is allocated at compile time
    // They use less memory than linked lists
    // Elements are accessed easily
}
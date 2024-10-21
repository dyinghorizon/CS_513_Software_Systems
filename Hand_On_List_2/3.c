/*
============================================================================
Name : 3.c
Author : Nishad Bagade
Question : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./3
Current stack size limit:
Stack size limit:
  Soft limit: 8388608
  Hard limit: -1

New stack size limit:
Stack size limit:
  Soft limit: 16777216
  Hard limit: 33554432
============================================================================
*/


#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

void check_limit(int resource, const char *resource_name) {
    struct rlimit limit;

    // Get the current resource limit using getrlimit
    if (getrlimit(resource, &limit) == 0) {
        printf("%s:\n", resource_name);
        printf("  Soft limit: %ld\n", (long)limit.rlim_cur);
        printf("  Hard limit: %ld\n\n", (long)limit.rlim_max);
    } else {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
}

int main() {
    struct rlimit new_limit;

    // Check current stack size limit
    printf("Current stack size limit:\n");
    check_limit(RLIMIT_STACK, "Stack size limit");

    // Set new stack size limit (example: 16 MB)
    new_limit.rlim_cur = 16 * 1024 * 1024; // Soft limit (16 MB)
    new_limit.rlim_max = 32 * 1024 * 1024; // Hard limit (32 MB)

    if (setrlimit(RLIMIT_STACK, &new_limit) != 0) {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }

    // Check new stack size limit
    printf("New stack size limit:\n");
    check_limit(RLIMIT_STACK, "Stack size limit");

    return 0;
}

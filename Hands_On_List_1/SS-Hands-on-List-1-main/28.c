/*
============================================================================
Name : 28.c
Author : Nishad Bagade
Description: Write a program to get maximum and minimum real time priority.
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./28
Real-Time Priority Limits:
Scheduling Policy: SCHED_FIFO
Maximum Priority: 99
Minimum Priority: 1
Scheduling Policy: SCHED_RR
Maximum Priority: 99
Minimum Priority: 1
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>

void print_priority_limits(int policy) {
    int max_priority, min_priority;

    // Get the maximum real-time priority for the given policy
    max_priority = sched_get_priority_max(policy);
    if (max_priority == -1) {
        perror("sched_get_priority_max");
        exit(EXIT_FAILURE);
    }

    // Get the minimum real-time priority for the given policy
    min_priority = sched_get_priority_min(policy);
    if (min_priority == -1) {
        perror("sched_get_priority_min");
        exit(EXIT_FAILURE);
    }

    printf("Scheduling Policy: ");
    switch (policy) {
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        default:
            printf("Unknown Policy\n");
            return;
    }
    
    printf("Maximum Priority: %d\n", max_priority);
    printf("Minimum Priority: %d\n", min_priority);
}

int main() {
    printf("Real-Time Priority Limits:\n");

    // Get and print priority limits for SCHED_FIFO
    print_priority_limits(SCHED_FIFO);

    // Get and print priority limits for SCHED_RR
    print_priority_limits(SCHED_RR);

    return 0;
}

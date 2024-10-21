/*
============================================================================
Name : 29.c
Author : Nishad Bagade
Description: Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ sudo ./29 1
[sudo] password for nishad: 
Current Scheduling Policy: SCHED_OTHER
Current Scheduling Policy: SCHED_FIFO
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ sudo ./29 2
Current Scheduling Policy: SCHED_OTHER
Current Scheduling Policy: SCHED_RR
============================================================================
*/

#include <stdio.h> // For standard input and output functions like printf
#include <stdlib.h> // For general utilities like exit
#include <sched.h> // For scheduling functions and constants
#include <unistd.h> 
#include <string.h> // For string handling functions
#include <errno.h> // For error handling

void print_policy(int policy) {
    switch (policy) {
        case SCHED_FIFO:
            printf("Current Scheduling Policy: SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("Current Scheduling Policy: SCHED_RR\n");
            break;
        case SCHED_OTHER:
            printf("Current Scheduling Policy: SCHED_OTHER\n");
            break;
        default:
            printf("Unknown Policy\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    struct sched_param param;
    int policy;
    int new_policy;

    // Get the current scheduling policy
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    // Print the current policy
    print_policy(policy);

    // Set the new policy
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <policy>\n", argv[0]);
        fprintf(stderr, "Policy: 1 for SCHED_FIFO, 2 for SCHED_RR\n");
        exit(EXIT_FAILURE);
    }

    new_policy = atoi(argv[1]);
    if (new_policy == 1) {
        new_policy = SCHED_FIFO;
    } else if (new_policy == 2) {
        new_policy = SCHED_RR;
    } else {
        fprintf(stderr, "Invalid policy. Use 1 for SCHED_FIFO, 2 for SCHED_RR\n");
        exit(EXIT_FAILURE);
    }

    // Set the new scheduling policy
    param.sched_priority = sched_get_priority_max(new_policy);
    if (sched_setscheduler(0, new_policy, &param) == -1) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    // sched_getscheduler(0) retrieves the scheduling policy of the current process (0 refers to the current process).
    // Confirm the new policy
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    print_policy(policy);

    return 0;
}

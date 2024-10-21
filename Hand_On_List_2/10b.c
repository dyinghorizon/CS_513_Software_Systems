/*
============================================================================
Name : 10b.c
Author : Nishad Bagade
Question : Write a separate program using sigaction system call to catch the following signals.
            b. SIGINT
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./10b
Waiting for SIGINT (Ctrl+C)... Press Ctrl+C to trigger the signal handler.
^CCaught signal 2 (SIGINT). You pressed Ctrl+C!
^CCaught signal 2 (SIGINT). You pressed Ctrl+C!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

// Signal handler function for SIGINT
void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT). You pressed Ctrl+C!\n", sig);
}

int main() {
    struct sigaction sa;

    // Set up the sigaction struct to handle SIGINT
    memset(&sa, 0, sizeof(sa));  // Clear the structure
    sa.sa_handler = handle_sigint;  // Set the signal handler function
    sa.sa_flags = 0;  // No special flags

    // Set the sigaction for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for SIGINT (Ctrl+C)... Press Ctrl+C to trigger the signal handler.\n");

    // Infinite loop to keep the program running and wait for the signal
    while (1) {
        sleep(1);  // Sleep for 1 second
    }

    return 0;
}

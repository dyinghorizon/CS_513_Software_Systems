/*
============================================================================
Name : 10c.c
Author : Nishad Bagade
Question : Write a separate program using sigaction system call to catch the following signals.
            c. SIGFPE
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./10c
Triggering a floating-point exception (division by zero)...
Caught signal 8 (SIGFPE). Floating-point exception occurred!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

// Signal handler for SIGFPE
void handle_sigfpe(int sig) {
    printf("Caught signal %d (SIGFPE). Floating-point exception occurred!\n", sig);
    exit(EXIT_FAILURE);  // Exit the program after handling the signal
}

int main() {
    struct sigaction sa;

    // Set up the sigaction struct to handle SIGFPE
    memset(&sa, 0, sizeof(sa));  // Clear the structure
    sa.sa_handler = handle_sigfpe;  // Set the signal handler function

    // Set the sigaction for SIGFPE
    if (sigaction(SIGFPE, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("Triggering a floating-point exception (division by zero)...\n");

    // Deliberately cause a floating-point exception (division by zero)
    int x = 5;
    int y = 0;
    int result = x / y;  // This will cause a SIGFPE signal (division by zero)

    // The following line will not be executed, as the program will terminate
    // after the signal is caught and handled.
    printf("Result: %d\n", result);

    return 0;
}

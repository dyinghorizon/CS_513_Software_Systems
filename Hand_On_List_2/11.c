/*
============================================================================
Name : 11.c
Author : Nishad Bagade
Question : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -
           use sigaction system call.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./11
SIGINT signal is now being ignored for 10 seconds. Try pressing Ctrl+C.
^CSIGINT (Ctrl+C) received but ignored.
SIGINT signal handler reset to default. Press Ctrl+C again.
^C
============================================================================
*/

#include <stdio.h>      // For printf
#include <signal.h>     // For sigaction, SIGINT
#include <unistd.h>     // For sleep, pause

// Function to handle and ignore SIGINT
void ignore_sigint(int sig_num) {
    printf("SIGINT (Ctrl+C) received but ignored.\n");
}

int main() {
    struct sigaction sa_ignore, sa_default;

    // Step 1: Set up the sigaction to ignore SIGINT
    sa_ignore.sa_handler = ignore_sigint;    // Set handler to ignore function
    sigemptyset(&sa_ignore.sa_mask);         // No additional signals are blocked
    sa_ignore.sa_flags = 0;                  // No special flags

    // Step 2: Apply the ignore action for SIGINT
    sigaction(SIGINT, &sa_ignore, NULL);
    printf("SIGINT signal is now being ignored for 10 seconds. Try pressing Ctrl+C.\n");

    // Wait for 10 seconds, during which SIGINT will be ignored
    sleep(10);

    // Step 3: Reset to default action for SIGINT
    sa_default.sa_handler = SIG_DFL;         // Set handler to default action
    sigemptyset(&sa_default.sa_mask);        // No additional signals are blocked
    sa_default.sa_flags = 0;                 // No special flags

    // Apply the default action for SIGINT
    sigaction(SIGINT, &sa_default, NULL);
    printf("SIGINT signal handler reset to default. Press Ctrl+C again.\n");

    // Wait indefinitely for a signal (SIGINT or others)
    pause();

    return 0;
}

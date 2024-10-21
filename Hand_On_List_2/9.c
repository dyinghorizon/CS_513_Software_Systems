/*
============================================================================
Name : 9.c
Author : Nishad Bagade
Question : Write a separate program using signal system call to catch the following signals.
                g. SIGPROF (use setitimer system call)
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./9
SIGINT signal is now being ignored for 10 seconds. Try pressing Ctrl+C.
^CSIGINT (Ctrl+C) received but ignored.
SIGINT signal handler reset to default. Press Ctrl+C again.
^CSIGINT (Ctrl+C) received, resetting to default action.
============================================================================
*/

#include <stdio.h>      // For printf
#include <signal.h>     // For signal handling functions
#include <unistd.h>     // For sleep

// Signal handler for SIGINT (Ctrl+C)
void ignore_sigint(int sig_num) {
    printf("SIGINT (Ctrl+C) received but ignored.\n");
}

// Reset the signal handling to default action
void reset_sigint(int sig_num) {
    printf("SIGINT (Ctrl+C) received, resetting to default action.\n");
    signal(SIGINT, SIG_DFL);  // Reset to default action for SIGINT
}

int main() {
    // Set the signal handler to ignore SIGINT
    signal(SIGINT, ignore_sigint);
    printf("SIGINT signal is now being ignored for 10 seconds. Try pressing Ctrl+C.\n");

    // Wait for 10 seconds while ignoring SIGINT
    sleep(10);

    // Reset the signal handler to default action for SIGINT
    signal(SIGINT, reset_sigint);
    printf("SIGINT signal handler reset to default. Press Ctrl+C again.\n");

    // Wait for SIGINT to reset to default action
    pause();  // Wait indefinitely for a signal (Ctrl+C)

    return 0;
}

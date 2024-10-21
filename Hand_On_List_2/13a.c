/*
============================================================================
Name : 13a.c
Author : Nishad Bagade
Question : Waiting to Catch Signals
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./13a
Process ID: 30441
Waiting for signals...

[1]+  Stopped                 ./13a

============================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGINT and other signals
void signal_handler(int sig_num) {
    if (sig_num == SIGINT) {
        printf("Caught SIGINT (Ctrl+C)\n");
    } else if (sig_num == SIGTERM) {
        printf("Caught SIGTERM\n");
    } else {
        printf("Caught signal %d\n", sig_num);
    }
}

int main() {
    // Register signal handlers
    signal(SIGINT, signal_handler);  // Can catch
    signal(SIGTERM, signal_handler); // Can catch
    signal(SIGTSTP, signal_handler); // Can catch SIGTSTP (Ctrl+Z)
    
    // Attempt to catch SIGSTOP (but it will fail)
    signal(SIGSTOP, signal_handler); // This won't work (SIGSTOP cannot be caught)

    // Print the process ID (PID) of this process
    pid_t pid = getpid();
    printf("Process ID: %d\n", pid);

    printf("Waiting for signals...\n");

    // Infinite loop to keep the program running
    while (1) {
        sleep(1);  // Sleep to simulate a running process
    }

    return 0;
}

/*
============================================================================
Name : 12.c
Author : Nishad Bagade
Question : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
           the parent process from the child process.
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./12
Parent process with PID: 6508
Child process created with PID: 6509
Child will now kill the parent process (PID: 6508)
Child process (orphan) is running with parent terminated.
Killed
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ Child process (orphan) is now adopted by init (or systemd) with PID 1.
^C
============================================================================
*/

#include <stdio.h>      // For printf
#include <unistd.h>     // For fork, getpid, sleep
#include <signal.h>     // For kill, SIGKILL
#include <stdlib.h>     // For exit

int main() {
    pid_t pid = fork();  // Create a child process using fork

    if (pid < 0) {
        // Error occurred in creating a process
        printf("Fork failed.\n");
        exit(1);
    } 
    else if (pid == 0) {
        // Child process
        printf("Child process created with PID: %d\n", getpid());
        printf("Child will now kill the parent process (PID: %d)\n", getppid());

        // Send SIGKILL to parent process to terminate it
        kill(getppid(), SIGKILL);

        // Sleep to demonstrate the orphan process (this delay shows that the child survives even after parent dies)
        printf("Child process (orphan) is running with parent terminated.\n");
        sleep(5);  // Wait for a few seconds
        printf("Child process (orphan) is now adopted by init (or systemd) with PID 1.\n");
    } 
    else {
        // Parent process
        printf("Parent process with PID: %d\n", getpid());

        // Sleep for some time so that child gets time to kill the parent
        sleep(10);  // Give enough time for child to act
    }

    return 0;
}

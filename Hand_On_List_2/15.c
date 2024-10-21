/*
============================================================================
Name : 15.c
Author : Nishad Bagade
Question : Write a simple program to send some data from parent to the child process.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./15
Child process received: Message from parent to child
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>  // Include this header for the `wait` function

int main() {
    int pipefds[2];  // Array to hold the file descriptors for the pipe
    pid_t pid;
    char write_msg[] = "Message from parent to child";
    char read_msg[100];

    // Create the pipe
    if (pipe(pipefds) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0) { 
        // Parent process
        close(pipefds[0]);  // Close unused read end of the pipe

        // Write to the pipe
        write(pipefds[1], write_msg, strlen(write_msg) + 1);  // +1 to include the null terminator
        close(pipefds[1]);  // Close the write end of the pipe after writing

        // Wait for the child process to complete
        wait(NULL);
    } else {  
        // Child process
        close(pipefds[1]);  // Close unused write end of the pipe

        // Read from the pipe
        read(pipefds[0], read_msg, sizeof(read_msg));
        printf("Child process received: %s\n", read_msg);

        close(pipefds[0]);  // Close the read end of the pipe after reading
    }

    return 0;
}

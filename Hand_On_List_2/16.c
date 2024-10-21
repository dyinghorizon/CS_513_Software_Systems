/*
============================================================================
Name : 16.c
Author : Nishad Bagade
Question : Write a program to send and receive data from parent to child vice versa. Use two way
           communication.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./16
Child received: Message from parent to child
Parent received: Message from child to parent
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];  // Two pipes: one for parent-to-child, one for child-to-parent
    pid_t pid;
    char parent_msg[] = "Message from parent to child";
    char child_msg[] = "Message from child to parent";
    char buffer[100];

    // Create first pipe (parent to child)
    if (pipe(pipe1) == -1) {
        perror("pipe1 failed");
        return 1;
    }

    // Create second pipe (child to parent)
    if (pipe(pipe2) == -1) {
        perror("pipe2 failed");
        return 1;
    }

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0) {  
        // Parent process
        close(pipe1[0]);  // Close the read end of pipe1 (parent to child)
        close(pipe2[1]);  // Close the write end of pipe2 (child to parent)

        // Write a message to the child
        write(pipe1[1], parent_msg, strlen(parent_msg) + 1);  // +1 for null terminator
        close(pipe1[1]);  // Close the write end of pipe1 after sending

        // Wait for the child to respond
        wait(NULL);

        // Read the child's response
        read(pipe2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(pipe2[0]);  // Close the read end of pipe2 after reading
    } else {  
        // Child process
        close(pipe1[1]);  // Close the write end of pipe1 (parent to child)
        close(pipe2[0]);  // Close the read end of pipe2 (child to parent)

        // Read the parent's message
        read(pipe1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipe1[0]);  // Close the read end of pipe1 after reading

        // Send a message back to the parent
        write(pipe2[1], child_msg, strlen(child_msg) + 1);  // +1 for null terminator
        close(pipe2[1]);  // Close the write end of pipe2 after sending
    }

    return 0;
}

/*
============================================================================
Name : 14.c
Author : Nishad Bagade
Question : Write a simple program to create a pipe, write to the pipe, 
           read from pipe and display on the monitor.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./14
Read from pipe: Hello from the pipe!
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefds[2];  // Array to hold the file descriptors for the pipe
    char write_msg[] = "Hello from the pipe!";
    char read_msg[100];
    
    // Create the pipe
    if (pipe(pipefds) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Write to the pipe
    write(pipefds[1], write_msg, strlen(write_msg) + 1);  // +1 to include null terminator

    // Read from the pipe
    read(pipefds[0], read_msg, sizeof(read_msg));

    // Display the message read from the pipe
    printf("Read from pipe: %s\n", read_msg);

    // Close the pipe
    close(pipefds[0]);
    close(pipefds[1]);

    return 0;
}

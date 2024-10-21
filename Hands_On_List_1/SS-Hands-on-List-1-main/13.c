/*
============================================================================
Name : 13.c
Author : Nishad Bagade
Question : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
           verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 14th - Aug - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./13
No data within 10 seconds.
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./13
hsadsjhfjsdhfbj
Data is available now.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h> // For `select` system call and file descriptor set macros
#include <sys/time.h> // For `struct timeval` to specify the timeout

//select() is a system call used to monitor multiple file descriptors (e.g., sockets, files, or stdin) 
//to see if they are ready for I/O operations like reading, writing, or exceptional conditions.

int main() {
    fd_set readfds;
    struct timeval tv;
    int retval;

    // Clear the set ahead of time
    FD_ZERO(&readfds);
    // Add our file descriptor (STDIN) to the set
    FD_SET(STDIN_FILENO, &readfds);

    // Set timeout to 10 seconds
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    // Wait for data on STDIN (file descriptor 0)
    retval = select(1, &readfds, NULL, NULL, &tv);

    if (retval == -1) {
        perror("select() failed");
        exit(EXIT_FAILURE);
    } else if (retval) {
        printf("Data is available now.\n");
    } else {
        printf("No data within 10 seconds.\n");
    }

    return 0;
}

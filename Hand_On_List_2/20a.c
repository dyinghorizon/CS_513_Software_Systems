/*
============================================================================
Name : 20a.c
Author : Nishad Bagade
Question : Write two programs so that both can communicate by FIFO -Use one way communications.
            Writer Program
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./20a
Data written to FIFO: Hello from the writer program!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    // Create the FIFO (named pipe) if it doesn't exist
    if (mkfifo(FIFO_FILE, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write some data to the FIFO
    const char *message = "Hello from the writer program!";
    write(fd, message, strlen(message) + 1);  // Include the null terminator

    // Close the FIFO
    close(fd);

    printf("Data written to FIFO: %s\n", message);
    return 0;
}

/*
============================================================================
Name : 22.c
Author : Nishad Bagade
Question : Write a program to wait for data to be written into FIFO within 10 seconds, use select
           system call with FIFO.
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./22
Waiting for data on FIFO for up to 10 seconds...
Received data: Hello, FIFO!
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./22
Waiting for data on FIFO for up to 10 seconds...
Timeout occurred. No data was written to the FIFO within 10 seconds.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "myfifo"
#define TIMEOUT_SECONDS 10

int main() {
    int fd;
    fd_set read_fds;
    struct timeval timeout;
    char buffer[100];
    int ret;

    // Create FIFO if it doesn't exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }

    // Open the FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Initialize the timeout
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    // Initialize the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    printf("Waiting for data on FIFO for up to %d seconds...\n", TIMEOUT_SECONDS);

    // Use select() to wait for data to be available for reading
    ret = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (ret == -1) {
        // Error occurred during select()
        perror("select failed");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (ret == 0) {
        // Timeout occurred
        printf("Timeout occurred. No data was written to the FIFO within %d seconds.\n", TIMEOUT_SECONDS);
    } else {
        // Data is available for reading
        if (FD_ISSET(fd, &read_fds)) {
            // Read the data from the FIFO
            ret = read(fd, buffer, sizeof(buffer));
            if (ret > 0) {
                buffer[ret] = '\0';
                printf("Received data: %s\n", buffer);
            } else {
                printf("Error reading from FIFO.\n");
            }
        }
    }

    // Close the FIFO
    close(fd);

    return 0;
}

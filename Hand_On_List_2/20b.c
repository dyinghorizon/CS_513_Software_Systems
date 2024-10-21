/*
============================================================================
Name : 20b.c
Author : Nishad Bagade
Question : Write two programs so that both can communicate by FIFO -Use two way communications.
            Reader Program
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./20b
Data read from FIFO: Hello from the writer program!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    // Open the FIFO for reading
    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Buffer to store the read message
    char buffer[100];

    // Read data from the FIFO
    int num_bytes = read(fd, buffer, sizeof(buffer));
    if (num_bytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the string and print the message
    buffer[num_bytes] = '\0';
    printf("Data read from FIFO: %s\n", buffer);

    // Close the FIFO
    close(fd);
    return 0;
}

/*
============================================================================
Name : 17_inc.c
Author : Nishad Bagade
Description: Program to Reserve a Ticket by Incrementing the Ticket Number
Date: 24nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./17_init
Initialized ticket number to 1000
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./17_inc
New ticket number: 1001
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char *filename = "ticket.dat";
    int fd;
    int ticket_number;

    // Open the file for reading and writing
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Implement a write lock
    struct flock lock;
    lock.l_type = F_WRLCK;   // Request a write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;        // Lock the entire file
    lock.l_len = 0;          // 0 means lock until the end of the file

    if (fcntl(fd, F_SETLKW, &lock) == -1) {  // F_SETLKW waits if the lock is unavailable
        perror("Error applying write lock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Read the current ticket number from the file
    if (read(fd, &ticket_number, sizeof(int)) == -1) {
        perror("Error reading from file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Increment the ticket number
    ticket_number++;

    // Move the file pointer back to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking in file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write the new ticket number back to the file
    if (write(fd, &ticket_number, sizeof(int)) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("New ticket number: %d\n", ticket_number);

    // Unlock the file by closing it
    close(fd);

    return 0;
}

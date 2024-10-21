/*
============================================================================
Name : 17_init.c
Author : Nishad Bagade
Description: Program to Initialize and Store a Ticket Number
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

int main() {
    const char *filename = "ticket.dat";
    int fd;
    int initial_ticket_number = 1000;

    // Open the file (create if it doesn't exist) and truncate it
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write the initial ticket number to the file
    if (write(fd, &initial_ticket_number, sizeof(int)) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Initialized ticket number to %d\n", initial_ticket_number);

    // Close the file
    close(fd);

    return 0;
}

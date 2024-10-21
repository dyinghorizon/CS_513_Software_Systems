/*
============================================================================
Name : 21a.c
Author : Nishad Bagade
Question : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./21a
Process 1: Enter message to send: Hello Nishad!
Process 1 received: Hello Linux!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main() {
    char message1[100], message2[100];
    int fd1, fd2;

    // Create FIFO1 and FIFO2
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Communication loop
    while (1) {
        // Write to FIFO1 (sending message)
        fd1 = open(FIFO1, O_WRONLY);
        printf("Process 1: Enter message to send: ");
        fgets(message1, 100, stdin);
        write(fd1, message1, strlen(message1) + 1);
        close(fd1);

        // Read from FIFO2 (receiving message)
        fd2 = open(FIFO2, O_RDONLY);
        read(fd2, message2, sizeof(message2));
        printf("Process 1 received: %s\n", message2);
        close(fd2);
    }

    return 0;
}

/*
============================================================================
Name : 21b.c
Author : Nishad Bagade
Question : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./21b
Process 2 received: Hello Nishad!

Process 2: Enter message to send: Hello Linux!
Process 2 received: Hello Nishad!
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

    // Communication loop
    while (1) {
        // Read from FIFO1 (receiving message)
        fd1 = open(FIFO1, O_RDONLY);
        read(fd1, message1, sizeof(message1));
        printf("Process 2 received: %s\n", message1);
        close(fd1);

        // Write to FIFO2 (sending message)
        fd2 = open(FIFO2, O_WRONLY);
        printf("Process 2: Enter message to send: ");
        fgets(message2, 100, stdin);
        write(fd2, message2, strlen(message2) + 1);
        close(fd2);
    }

    return 0;
}

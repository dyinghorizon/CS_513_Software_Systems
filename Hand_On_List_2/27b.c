/*
============================================================================
Name : 27b.c
Author : Nishad Bagade
Question : Write a program to receive messages from the message queue.
            b. with IPC_NOWAIT as a flag
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./27b
Received message: Hello!
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key;
    int msgid;

    // Generate a unique key
    key = ftok("progfile", 65);

    // Create or access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Try to receive message (do not wait)
    if (msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    // Display the message if received
    printf("Received message: %s\n", message.msg_text);

    return 0;
}

/*
============================================================================
Name : 29.c
Author : Nishad Bagade
Question : Write a program to remove the message queue.
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./29
Message queue created with ID: 0
Message queue removed successfully.
============================================================================
*/

#include <stdio.h>      // Standard input/output library
#include <stdlib.h>     // For exit function
#include <sys/ipc.h>    // For IPC_CREAT, IPC_RMID, key_t
#include <sys/msg.h>    // For message queue functions like msgget, msgctl

int main() {
    key_t key;
    int msgid;

    // Step 1: Generate a unique key for the message queue
    key = ftok("progfile", 65);  // 'progfile' can be any existing file, 65 is the project ID
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Step 2: Create or get the message queue ID
    msgid = msgget(key, 0666 | IPC_CREAT);  // 0666 provides read/write permissions
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Message queue created with ID: %d\n", msgid);

    // Step 3: Remove the message queue using msgctl
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl (remove queue) failed");
        exit(1);
    }

    printf("Message queue removed successfully.\n");

    return 0;
}

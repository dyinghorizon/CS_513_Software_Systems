/*
============================================================================
Name : 24.c
Author : Nishad Bagade
Question : Write a program to create a message queue and print the key and message queue id.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./24
Message queue created.
Key: 1091118187
Message Queue ID: 0
============================================================================
*/

#include <stdio.h>      // For printf and perror
#include <stdlib.h>     // For exit function
#include <sys/ipc.h>    // For IPC_CREAT, key_t
#include <sys/msg.h>    // For msgget function and message queue operations

int main() {
    key_t key;   // Variable to store the key for the message queue
    int msgid;   // Variable to store the message queue ID

    // Step 1: Generate a unique key for the message queue
    // The "progfile" can be any existing file, and 65 is the project ID.
    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok failed");   // Error handling in case ftok fails
        exit(1);
    }

    // Step 2: Create the message queue or access an existing one with the same key
    // 0666 allows read and write permissions for all users.
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");  // Error handling in case msgget fails
        exit(1);
    }

    // Step 3: Print the key and message queue ID
    printf("Message queue created.\n");
    printf("Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}

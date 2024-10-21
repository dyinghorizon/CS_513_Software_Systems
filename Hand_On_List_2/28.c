/*
============================================================================
Name : 28.c
Author : Nishad Bagade
Question : Write a program to change the exiting message queue permission. (use msqid_ds structure)
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./28
Current Permissions: 666
Updated Permissions: 644
============================================================================
*/

#include <stdio.h>      // For printf and perror
#include <stdlib.h>     // For exit function
#include <sys/ipc.h>    // For key_t, IPC_CREAT
#include <sys/msg.h>    // For message queue operations

int main() {
    key_t key;             // Key for the message queue
    int msgid;             // Message queue ID
    struct msqid_ds buf;   // Buffer to hold message queue information

    // Step 1: Generate a unique key for the message queue
    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Step 2: Get the message queue ID (create if doesn't exist)
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Step 3: Get the current permissions of the message queue
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl (IPC_STAT) failed");
        exit(1);
    }

    // Step 4: Print the current permissions
    printf("Current Permissions: %o\n", buf.msg_perm.mode);

    // Step 5: Change the permission of the message queue
    buf.msg_perm.mode = 0644;  // Setting read/write for owner and read for others

    // Step 6: Update the message queue with the new permissions
    if (msgctl(msgid, IPC_SET, &buf) == -1) {
        perror("msgctl (IPC_SET) failed");
        exit(1);
    }

    // Step 7: Get the new permissions to verify the change
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl (IPC_STAT) failed");
        exit(1);
    }

    // Step 8: Print the updated permissions
    printf("Updated Permissions: %o\n", buf.msg_perm.mode);

    return 0;
}

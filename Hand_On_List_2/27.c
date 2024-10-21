/*
============================================================================
Name : 27.c
Author : Nishad Bagade
Question : Sending a Message (For Testing)
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./27
Enter a message: Hello, Nishad!
Message sent: Hello, Nishad!
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

    // Message type
    message.msg_type = 1;

    // Input message text
    printf("Enter a message: ");
    fgets(message.msg_text, sizeof(message.msg_text), stdin);

    // Send the message
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Message sent: %s", message.msg_text);

    return 0;
}

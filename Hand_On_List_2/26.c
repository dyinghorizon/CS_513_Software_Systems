/*
============================================================================
Name : 26.c
Author : Nishad Bagade
Question : Write a program to send messages to the message queue. Check $ipcs -q
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./26
Message sent! Check using `ipcs -q`
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x4109246b 0          nishad     666        0            0           
0x010931b1 1          nishad     700        16           1 
============================================================================
*/

#include <sys/types.h> // Import for `ftok` `msgget` & `msgsnd`
#include <sys/ipc.h>   // Import for `ftok` `msgget` & `msgsnd`
#include <sys/msg.h>   // Import for `msgget` & `msgsnd`
#include <stdio.h>     // Import for `perror` & `printf`
#include <unistd.h>    // Import for `_exit`
#include <errno.h>     // Import for `errno`

void main()
{
    key_t queueKey;        // IPC (Message Queue) key
    int queueIdentifier;   // IPC (Message Queue) identifier
    int messageSendStatus; // Determines success of `msgsnd` call

    struct msgbuf
    {
        long mtype;     // Used to identify message type - should be > 0
        int someNumber; // Integer data
    } data;

    queueKey = ftok(".", 1);

    if (queueKey == -1)
    {
        perror("Error while computing key!");
        _exit(0);
    }

    queueIdentifier = msgget(queueKey, IPC_CREAT | 0700);

    if (queueIdentifier == -1)
    {
        perror("Error while creating message queue!");
        _exit(0);
    }

    data.mtype = 1;
    data.someNumber = 100;

    messageSendStatus = msgsnd(queueIdentifier, &data, sizeof(data), 0);

    if (messageSendStatus == -1)
    {
        perror("Error while sending message via Message Queue!");
        _exit(0);
    }

    printf("Message sent! Check using `ipcs -q`\n");
}
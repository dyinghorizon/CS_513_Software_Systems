/*
============================================================================
Name : 32a.c
Author : Nishad Bagade
Question : Write a program to implement semaphore to protect any critical section.
            a. rewrite the ticket number creation program using semaphore
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32a
Ticket issued: 1
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32a
Ticket issued: 2
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32a
Ticket issued: 3
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

// Define union semun for semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void semaphore_wait(int semid) {
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;  // P operation
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

void semaphore_signal(int semid) {
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;  // V operation
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

int main() {
    key_t key = ftok("ticketfile", 65);  // Create a file first: touch ticketfile
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    // Initialize the semaphore with value 1 (binary semaphore)
    union semun sem_union;
    sem_union.val = 1;
    semctl(semid, 0, SETVAL, sem_union);

    // Open or create a file to store the ticket number
    int ticket_fd = open("ticket.txt", O_RDWR | O_CREAT, 0666);

    int ticket;
    semaphore_wait(semid);  // Enter critical section

    read(ticket_fd, &ticket, sizeof(ticket));
    ticket++;
    lseek(ticket_fd, 0, SEEK_SET);
    write(ticket_fd, &ticket, sizeof(ticket));
    printf("Ticket issued: %d\n", ticket);

    semaphore_signal(semid);  // Exit critical section

    close(ticket_fd);
    return 0;
}

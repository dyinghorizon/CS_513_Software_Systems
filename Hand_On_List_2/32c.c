/*
============================================================================
Name : 32c.c
Author : Nishad Bagade
Question : Write a program to implement semaphore to protect any critical section.
            c. protect multiple pseudo resources ( may be two) using counting semaphore
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32c
Accessing resource...
Resource acquired.
Releasing resource...
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

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
    key_t key = ftok("resourcefile", 65);  // Create a file first: touch resourcefile
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    // Initialize counting semaphore with 2 (representing 2 resources)
    union semun sem_union;
    sem_union.val = 2;
    semctl(semid, 0, SETVAL, sem_union);

    printf("Accessing resource...\n");
    semaphore_wait(semid);  // Enter critical section
    printf("Resource acquired.\n");

    // Simulate work with the resource
    sleep(2);

    printf("Releasing resource...\n");
    semaphore_signal(semid);  // Exit critical section

    return 0;
}

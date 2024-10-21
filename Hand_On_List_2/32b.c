/*
============================================================================
Name : 32b.c
Author : Nishad Bagade
Question : Write a program to implement semaphore to protect any critical section.
            b. protect shared memory from concurrent write access
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32b
Write data to shared memory: Ticket
Data in shared memory: Ticket
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>

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
    key_t key = ftok("shmfile", 65);  // Create a file first: touch shmfile
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    // Initialize the semaphore with value 1 (binary semaphore)
    union semun sem_union;
    sem_union.val = 1;
    semctl(semid, 0, SETVAL, sem_union);

    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *) shmat(shmid, NULL, 0);

    semaphore_wait(semid);  // Enter critical section
    printf("Write data to shared memory: ");
    fgets(str, 1024, stdin);
    semaphore_signal(semid);  // Exit critical section

    printf("Data in shared memory: %s\n", str);

    shmdt(str);
    return 0;
}

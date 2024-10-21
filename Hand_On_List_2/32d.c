/*
============================================================================
Name : 32d.c
Author : Nishad Bagade
Question : Write a program to implement semaphore to protect any critical section.
            d. remove the created semaphore
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./32d
Semaphore removed successfully.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Define union semun for semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    key_t key = ftok("resourcefile", 65);  // Ensure to use the same file used for the semaphore
    int semid = semget(key, 1, 0666);

    // Remove the semaphore
    if (semctl(semid, 0, IPC_RMID, NULL) == -1) {
        perror("Failed to remove semaphore");
        exit(1);
    }

    printf("Semaphore removed successfully.\n");
    return 0;
}

/*
============================================================================
Name : 31a.c
Author : Nishad Bagade
Question : a. create a binary semaphore
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./31a
Binary semaphore created with ID: 0
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

// Function to initialize the binary semaphore
void initialize_semaphore(int semid) {
    // Setting the semaphore's value to 1 (binary semaphore)
    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        exit(1);
    }
}

int main() {
    key_t key;
    int semid;

    // Step 1: Generate a unique key for the semaphore
    key = ftok("binary_semfile", 65); // Create a file first using touch command
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Step 2: Create a binary semaphore (1 semaphore, IPC_CREAT to create if not exists)
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    // Step 3: Initialize the binary semaphore to 1 (indicating "unlocked" or "available")
    initialize_semaphore(semid);

    printf("Binary semaphore created with ID: %d\n", semid);

    return 0;
}

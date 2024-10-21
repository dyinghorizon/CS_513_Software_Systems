/*
============================================================================
Name : 31b.c
Author : Nishad Bagade
Question : b. create a counting semaphore
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./31b
Counting semaphore created with ID: 1 and initial value: 5
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

// Function to initialize the counting semaphore
void initialize_semaphore(int semid, int initial_value) {
    // Setting the semaphore's value to the provided initial value
    if (semctl(semid, 0, SETVAL, initial_value) == -1) {
        perror("semctl failed");
        exit(1);
    }
}

int main() {
    key_t key;
    int semid;

    // Step 1: Generate a unique key for the semaphore
    key = ftok("counting_semfile", 75); // Create a file first using touch command
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Step 2: Create a counting semaphore (1 semaphore, IPC_CREAT to create if not exists)
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    // Step 3: Initialize the counting semaphore with an initial value (e.g., 5 resources available)
    initialize_semaphore(semid, 5);

    printf("Counting semaphore created with ID: %d and initial value: 5\n", semid);

    return 0;
}

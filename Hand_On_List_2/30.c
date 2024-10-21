/*
============================================================================
Name : 30.c
Author : Nishad Bagade
Question : Write a program to create a shared memory.
            a. write some data to the shared memory
            b. attach with O_RDONLY and check whether you are able to overwrite.
            c. detach the shared memory
            d. remove the shared memory
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./30
Data written to shared memory: Hello, Shared Memory!
Attempting to overwrite data in read-only mode...
Segmentation fault (core dumped)
============================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Define a key and size for the shared memory
#define SHM_KEY 12345
#define SHM_SIZE 1024

int main() {
    int shm_id;
    char *shm_ptr;
    char *read_only_ptr;

    // Step a: Create the shared memory segment
    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);  // IPC_CREAT to create if it doesn't exist, 0666 permissions
    if (shm_id == -1) {
        perror("Failed to create shared memory");
        exit(1);
    }

    // Attach to the shared memory for writing
    shm_ptr = (char *) shmat(shm_id, NULL, 0);  // Attach with read-write mode
    if (shm_ptr == (char *)-1) {
        perror("Failed to attach shared memory");
        exit(1);
    }

    // Write some data to the shared memory
    strcpy(shm_ptr, "Hello, Shared Memory!");
    printf("Data written to shared memory: %s\n", shm_ptr);

    // Step b: Attach the shared memory segment in read-only mode
    read_only_ptr = (char *) shmat(shm_id, NULL, SHM_RDONLY);  // Attach with read-only mode
    if (read_only_ptr == (char *)-1) {
        perror("Failed to attach shared memory in read-only mode");
        exit(1);
    }

    // Try to overwrite data in read-only mode
    printf("Attempting to overwrite data in read-only mode...\n");
    strcpy(read_only_ptr, "Trying to overwrite");  // This will cause a segmentation fault if overwritten

    printf("Data in shared memory after overwrite attempt: %s\n", read_only_ptr);

    // Step c: Detach the shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("Failed to detach shared memory (write mode)");
        exit(1);
    }
    if (shmdt(read_only_ptr) == -1) {
        perror("Failed to detach shared memory (read-only mode)");
        exit(1);
    }

    // Step d: Remove the shared memory segment
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("Failed to remove shared memory");
        exit(1);
    }

    printf("Shared memory removed successfully.\n");

    return 0;
}

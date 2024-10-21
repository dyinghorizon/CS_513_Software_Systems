/*
============================================================================
Name : 16.c
Author : Nishad Bagade
Question : Write a program to perform mandatory locking.
            a. Implement write lock
            b. Implement read lock
Date: 22nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./16
Attempting to apply a write lock...
Write lock applied successfully.
Performing write operation...
Write lock released.
Attempting to apply a read lock...
Read lock applied successfully.
Performing read operation...
Read data: Hello, Nishad!

Read lock released.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// Function to implement a write lock
void write_lock(int fd) {
    struct flock lock;
    lock.l_type = F_WRLCK;   // Request a write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;        // Lock the entire file
    lock.l_len = 0;          // 0 means lock until the end of the file

    // Apply the write lock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("File is already locked by another process for writing.\n");
        } else {
            perror("Error applying write lock");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Write lock applied successfully.\n");
    }
}

// Function to implement a read lock
void read_lock(int fd) {
    struct flock lock;
    lock.l_type = F_RDLCK;   // Request a read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;        // Lock the entire file
    lock.l_len = 0;          // 0 means lock until the end of the file

    // Apply the read lock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("File is already locked by another process.\n");
        } else {
            perror("Error applying read lock");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Read lock applied successfully.\n");
    }
}

int main() {
    const char *filename = "testfile.txt";
    int fd;

    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Attempting to apply a write lock...\n");
    write_lock(fd);

    // Processing
    printf("Performing write operation...\n");
    write(fd, "Hello, Nishad!\n", 14);

    close(fd);
    printf("Write lock released.\n");

    // Reopen the file to apply a read lock
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Attempting to apply a read lock...\n");
    read_lock(fd);

    // Processing
    printf("Performing read operation...\n");
    char buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Read data: %s\n", buffer);
    } else {
        perror("Error reading file");
    }

    
    close(fd);
    printf("Read lock released.\n");

    return 0;
}

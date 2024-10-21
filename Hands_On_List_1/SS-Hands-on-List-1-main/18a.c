/*
============================================================================
Name : 18a.c
Author : Nishad Bagade
Description: Program to Implement Write Lock 
Date: 24nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18
Initialized records in the file.
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18a
Modified record 2 to new value: 200
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18b
Read record 2: value = 200
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define RECORD_SIZE sizeof(int)  // Each record is an integer

void write_lock(int fd, int record_num) {
    struct flock lock;
    lock.l_type = F_WRLCK;   // Request a write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;  // Lock the specific record
    lock.l_len = RECORD_SIZE;  // Lock one record

    // Apply the write lock, wait if necessary
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error applying write lock");
        exit(EXIT_FAILURE);
    }
}

void unlock(int fd, int record_num) {
    struct flock lock;
    lock.l_type = F_UNLCK;   // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;  // Unlock the specific record
    lock.l_len = RECORD_SIZE;  // Unlock one record

    // Apply the unlock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error unlocking");
        exit(EXIT_FAILURE);
    }
}

int main() {
    const char *filename = "records.dat";
    int fd;
    int record_num = 1;  // Modify the second record (index 1)
    int new_value = 200;

    // Open the file for reading and writing
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Lock the record for writing
    write_lock(fd, record_num);

    // Move to the specific record
    if (lseek(fd, record_num * RECORD_SIZE, SEEK_SET) == -1) {
        perror("Error seeking in file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write new value to the record
    if (write(fd, &new_value, RECORD_SIZE) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Modified record %d to new value: %d\n", record_num + 1, new_value);

    // Unlock the record
    unlock(fd, record_num);

    // Close the file
    close(fd);

    return 0;
}

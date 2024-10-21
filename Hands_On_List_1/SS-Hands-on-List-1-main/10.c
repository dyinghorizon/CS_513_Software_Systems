/*
============================================================================
Name : 10.c
Author : Nishad Bagade
Question : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
           bytes (use lseek) and write again 10 bytes.
            a. check the return value of lseek
            b. open the file with od and check the empty spaces in between the data.
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./10
lseek returned: 20
============================================================================
*/
#include <fcntl.h>  // Import for open, O_RDWR, O_CREAT
#include <unistd.h> // Import for write, lseek, close
#include <stdio.h>  // Import for printf
#include <stdlib.h> // Import for exit, EXIT_FAILURE

int main() {
    int fd;
    ssize_t bytes_written;
    off_t offset;
    char data1[10] = "ABCDEFGHIJ"; // First 10 bytes of data
    char data2[10] = "1234567890"; // Second 10 bytes of data

    // Open the file in read-write mode, create it if it doesn't exist
    fd = open("q10.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write the first 10 bytes to the file
    bytes_written = write(fd, data1, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Move the file pointer by 10 bytes
    offset = lseek(fd, 10, SEEK_CUR);
    if (offset == (off_t)-1) {
        perror("Error using lseek");
        close(fd);
        exit(EXIT_FAILURE);
    } else {
        printf("lseek returned: %ld\n", offset);
    }

    // Write the second 10 bytes to the file
    bytes_written = write(fd, data2, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    
    close(fd);

    return 0;
}

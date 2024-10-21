/*
============================================================================
Name : 7.c
Author : Nishad Bagade
Question : Write a program to copy file1 into file2 ($cp file1 file2).
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./7 file1 file2
============================================================================
*/

#include <fcntl.h>    // For open
#include <unistd.h>   // For read, write, and close
#include <sys/stat.h> // For file permissions

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sourceFile, destFile;
    ssize_t bytesRead, bytesWritten;
    char buffer[BUFFER_SIZE];

    if (argc != 3) {
        write(2, "Usage: ./cp file1 file2\n", 24);
        return 1;
    }

    // Open the source file (file1) in read-only mode
    sourceFile = open(argv[1], O_RDONLY);
    if (sourceFile < 0) {
        write(2, "Error opening source file\n", 26);
        return 1;
    }

    // Open the destination file (file2) in write-only mode, create it if it doesn't exist, truncate it if it does
    destFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (destFile < 0) {
        write(2, "Error opening destination file\n", 31);
        close(sourceFile);
        return 1;
    }

    // Read from source file and write to destination file
    while ((bytesRead = read(sourceFile, buffer, BUFFER_SIZE)) > 0) {
        bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            write(2, "Error writing to destination file\n", 34);
            close(sourceFile);
            close(destFile);
            return 1;
        }
    }

    close(sourceFile);
    close(destFile);

    return 0;
}


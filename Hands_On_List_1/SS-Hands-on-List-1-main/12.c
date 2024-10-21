/*
============================================================================
Name : 12.c
Author : Nishad Bagade
Question : Write a program to find out the opening mode of a file. Use fcntl.
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./12 file_1
File is opened in read-write mode.
File is opened in append mode.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h> // Import for `fcntl` and file control options
#include <unistd.h> // Import for `close`

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s file_2\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("Error opening the file");
        return 1;
    }

    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Error getting file flags");
        close(fd);
        return 1;
    }

    // Check access mode (O_RDONLY, O_WRONLY, O_RDWR)
    switch (flags & O_ACCMODE) {
        case O_RDONLY:
            printf("File is opened in read-only mode.\n");
            break;
        case O_WRONLY:
            printf("File is opened in write-only mode.\n");
            break;
        case O_RDWR:
            printf("File is opened in read-write mode.\n");
            break;
        default:
            printf("Unknown access mode.\n");
    }

    // Check additional flags
    if (flags & O_APPEND) {
        printf("File is opened in append mode.\n");
    }
    if (flags & O_NONBLOCK) {
        printf("File is opened in non-blocking mode.\n");
    }
    if (flags & O_SYNC) {
        printf("File is opened in synchronous mode.\n");
    }
    if (flags & O_CREAT) {
        printf("File is opened with O_CREAT flag.\n");
    }

    close(fd);
    return 0;
}

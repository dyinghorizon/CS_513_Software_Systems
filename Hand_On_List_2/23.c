/*
============================================================================
Name : 23.c
Author : Nishad Bagade
Question : Write a program to print the maximum number of files can be opened within a process and
            size of a pipe (circular buffer).
Date: 19th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./23
Maximum number of open files within a process: 1048576
Size of a pipe (circular buffer): 4096 bytes
============================================================================
*/

#include <stdio.h>      // For printf and perror
#include <unistd.h>     // For sysconf and pathconf
#include <limits.h>     // For path configuration constants

int main() {
    long max_open_files;
    long pipe_size;

    // Step 1: Get the maximum number of files that can be opened by a process
    max_open_files = sysconf(_SC_OPEN_MAX);
    if (max_open_files == -1) {
        perror("sysconf for _SC_OPEN_MAX failed");
        return 1;
    }

    // Step 2: Get the size of the pipe buffer (circular buffer)
    // Using "/dev/null" as the path argument since we're checking system-wide limits
    pipe_size = pathconf("/dev/null", _PC_PIPE_BUF);
    if (pipe_size == -1) {
        perror("pathconf for _PC_PIPE_BUF failed");
        return 1;
    }

    // Step 3: Print the results
    printf("Maximum number of open files within a process: %ld\n", max_open_files);
    printf("Size of a pipe (circular buffer): %ld bytes\n", pipe_size);

    return 0;
}

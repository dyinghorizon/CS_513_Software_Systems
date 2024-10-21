/*
============================================================================
Name : 1a.c
Author : Nishad Bagade
Question : a. soft link (symlink system call)
Date: 10th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./1a
Symbolic link created.
============================================================================
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    if (symlink("/home/nishad/Desktop/Hand On List 1/file_1", "/home/nishad/Desktop/Hand On List 1/f1/f2/symlink.txt") == -1) {
        perror("symlink");
        return 1;
    }

    printf("Symbolic link created.\n");
    return 0;
}

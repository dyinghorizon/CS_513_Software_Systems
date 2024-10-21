/*
============================================================================
Name : 1b.c
Author : Nishad Bagade
Question : b. hard link (link system call)
Date: 10th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./1b
Hard link created.
============================================================================
*/

#include <unistd.h>
#include <stdio.h>

int main() {
    if (link("/home/nishad/Desktop/Hand On List 1/file_1", "/home/nishad/Desktop/Hand On List 1/f1/f2/hardlink.txt") == -1) {
        perror("link");
        return 1;
    }

    printf("Hard link created.\n");
    return 0;
}

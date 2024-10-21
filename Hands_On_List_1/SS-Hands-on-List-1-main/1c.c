/*
============================================================================
Name : 1c.c
Author : Nishad Bagade
Question : c. FIFO (mkfifo Library Function or mknod system call)
Date: 10th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./1c
FIFO created.
============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main() {
    if (mkfifo("/home/nishad/Desktop/Hand On List 1/f1/f2/myfifo", 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    printf("FIFO created.\n");
    return 0;
}

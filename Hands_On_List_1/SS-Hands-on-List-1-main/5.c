/*
============================================================================
Name : 5.c
Author : Nishad Bagade
Question : Write a program to create five new files with infinite loop. 
           Execute the program in the background and check the file descriptor 
           table at /proc/pid/fd.
Date: 10th - Aug - 2024

Output: 
 nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./5
Created and opened file: file1.txt with fd: 3
Created and opened file: file2.txt with fd: 4
Created and opened file: file3.txt with fd: 5
Created and opened file: file4.txt with fd: 6
Created and opened file: file5.txt with fd: 7

============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filenames[5] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};
    int fds[5];
    
    // Create five new files
    for (int i = 0; i < 5; i++) {
        fds[i] = open(filenames[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fds[i] == -1) {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }
        printf("Created and opened file: %s with fd: %d\n", filenames[i], fds[i]);
    }

    // Infinite loop to keep the program running
    while (1) {
        sleep(1);  // Sleep to reduce CPU usage
    }

    // Close the file descriptors (this part won't be reached due to the infinite loop)
    for (int i = 0; i < 5; i++) {
        close(fds[i]);
    }

    return 0;
}

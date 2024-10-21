/*
============================================================================
Name : 17a.c
Author : Nishad Bagade
Question : Write a program to execute ls -l | wc.
            a. use dup
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./17a
total 1072
-rw-rw-r-- 1 nishad nishad  1277 Sep 18 23:08 10a.c
-rw-rw-r-- 1 nishad nishad   684 Sep 18 23:07 10b.c
-rw-rw-r-- 1 nishad nishad   684 Sep 18 23:07 10c.c
-rwxrwxr-x 1 nishad nishad 16216 Sep 18 23:11 11
-rw-rw-r-- 1 nishad nishad  2045 Sep 19 15:59 11.c
..
..
..
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main() {
    int pipefds[2];  // Create pipe
    pid_t pid;

    // Create the pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();  // Fork the process

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (ls -l)
        close(pipefds[0]);  // Close read end of the pipe

        // Duplicate write end of the pipe to stdout
        dup(pipefds[1]);

        // Close the original write end of the pipe
        close(pipefds[1]);

        // Execute `ls -l`
        execlp("ls", "ls", "-l", NULL);

        // If execlp fails
        perror("execlp ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent process (wc)
        close(pipefds[1]);  // Close write end of the pipe

        // Fork again to create a new process for wc
        if (fork() == 0) {
            // Child process (wc)
            dup(pipefds[0]);  // Duplicate read end of the pipe to stdin
            close(pipefds[0]);  // Close the original read end

            // Execute `wc`
            execlp("wc", "wc", NULL);

            // If execlp fails
            perror("execlp wc");
            exit(EXIT_FAILURE);
        }

        // Parent process closes both pipe ends
        close(pipefds[0]);

        // Wait for both child processes to complete
        wait(NULL);
        wait(NULL);
    }

    return 0;
}

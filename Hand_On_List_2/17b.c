/*
============================================================================
Name : 17b.c
Author : Nishad Bagade
Question : Write a program to execute ls -l | wc.
            b. use dup2
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./17b
    122    1091    6263
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

        // Duplicate write end of the pipe to stdout using dup2
        dup2(pipefds[1], STDOUT_FILENO);

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
            dup2(pipefds[0], STDIN_FILENO);  // Duplicate read end of the pipe to stdin
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

/*
============================================================================
Name : 23.c
Author : Nishad Bagade
Question : Write a program to create a Zombie state of the running program.
Date: 28th - Aug - 2024

Output:
nishad     14971  0.0  0.0   2552  1152 pts/2    S    15:46   0:00 ./23
nishad     14973  0.0  0.0      0     0 pts/2    Z    15:46   0:00 [23] <defunct>

============================================================================
*/
#include <stdlib.h> // exit()
#include <sys/types.h> // pid_t datatype
#include <unistd.h> // fork, sleep
#include <stdio.h> //printf

int main()
{
        pid_t child_pid;

        child_pid = fork();
        if (child_pid > 0)
        {
                printf("Child PID is %d\n", child_pid);
                sleep(60);
        }
        else
        {
                exit(0);
        }
        return 0;
}
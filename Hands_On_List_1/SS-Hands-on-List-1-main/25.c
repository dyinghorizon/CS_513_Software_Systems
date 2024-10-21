/*
============================================================================
Name : 25.c
Author : Nishad Bagade
Question : Write a program to create three child processes. The parent should 
           wait for a particular child (use waitpid system call).
Date: 30th - Aug - 2024

Output: 
Child I created...
Child I before 5 sec sleep
Child II created...
Child II befor 10 sec sleep
Child III created...
Child III before 15 sec sleep
Child I is now awake.
Child II is now awake.
Child III is now awake.
Child III has terminated with exit status = 0

============================================================================
*/

#include <unistd.h> // fork
#include <sys/types.h> // fork, waitpid
#include <sys/wait.h> // waitpid
#include <stdio.h> // printf

void main()
{
    pid_t child_I, child_II, child_III, terminated_child;
    int waitStatus;

    if((child_I = fork())== 0)
    {
        // Child I will run following code
        printf("Child I created...\n");
        printf("Child I before 5 sec sleep\n");
        sleep(5);
        printf("Child I is now awake.\n");
        _exit(0);
    }
    else
    {
        // Parent
        if ((child_II = fork()) == 0)
        {
            // Child II
            printf("Child II created...\n");
            printf("Child II befor 10 sec sleep\n");
            sleep(10);
            printf("Child II is now awake.\n");
            _exit(0);
        }
        else
        {
            // Parent
            if ((child_III = fork()) == 0)
            {
                // Child III
                printf("Child III created...\n");
                printf("Child III before 15 sec sleep\n");
                sleep(15);
                printf("Child III is now awake.\n");
                _exit(0);
            }
            else
            {
                // Parent
                // waitpid() suspends the parent process until the specified child process (child_III) changes its state (in this case, when it exits).
                terminated_child = waitpid(child_III, &waitStatus, 0);

                if (terminated_child == child_III)
                {
                    if (waitStatus == 0)
                    printf("Child III has terminated with exit status = 0\n");
                    else
                    printf("Child has exited with exit status : %d\n", waitStatus);
                }
            }
        }
    }
}

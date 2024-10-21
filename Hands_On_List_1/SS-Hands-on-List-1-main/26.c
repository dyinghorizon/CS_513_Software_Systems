/*
============================================================================
Name : 26.c
Author : Nishad Bagade
Question : Write a program to execute an executable program.
            a. use some executable program
            b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 30th - Aug - 2024

Output: 

============================================================================
*/

#include <unistd.h> // Import for `execl` library function

void main()
{
    char *executable_path = "./demo.out";
    char *arg = "file_1";

    execl(executable_path, executable_path, arg, NULL);
}
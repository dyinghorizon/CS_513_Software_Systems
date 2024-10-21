/*
============================================================================
Name : 27c.c
Author : Nishad Bagade
Description: Write a program to execute ls -Rl c. execle
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./27a
-rw-rw-r-- 2 nishad nishad 111 Aug 30 14:59 ./file_1
============================================================================
*/

#include <unistd.h> // Import for `execle` function
#include <stdio.h>  // Import for `printf` function

void main()
{
    char *command_path = "/bin/ls";
    char *options = "-Rl";
    char *file_arg = "./sample-files"; // Set if you want to execute `ls` command on a particular file

    execle(command_path, command_path, options, file_arg, NULL, NULL);
    printf("\n");
}

//execle is like execl, but it allows you to specify a custom environment (envp) for the new process. 
//This is useful if you want the new program to have a different set of environment variables.

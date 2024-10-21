/*
============================================================================
Name : 27e.c
Author : Nishad Bagade
Description: Write a program to execute ls -Rl e. execvp
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./27e
--wx-wS--- 1 nishad nishad 111 Aug 31 18:59 ./file_2
============================================================================
*/

#include <unistd.h> // Import for `execvp` function
#include <stdio.h>  // Import for `printf` function

void main()
{
    // Command name, options, file name,
    char *args[] = {"/bin/ls", "-Rl", "./file_2", NULL}; // Set the last but one argument if you want to execute `ls` command on a particular file

    execvp(args[0], args);
    printf("\n");
}

//execvp is like execv, but it searches for the program in the directories listed in the PATH environment variable, similar to execlp. 
//You pass the arguments as an array (argv[]).

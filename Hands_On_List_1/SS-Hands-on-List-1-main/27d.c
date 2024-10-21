/*
============================================================================
Name : 27d.c
Author : Nishad Bagade
Description: Write a program to execute ls -Rl d. execv
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./27d
--wx-wS--- 1 nishad nishad 111 Aug 31 18:59 ./file_2
============================================================================
*/

#include <unistd.h> // Import for `execv` function
#include <stdio.h>  // Import for `printf` function

void main()
{
    char *args[] = {"/bin/ls", "-Rl", "./file_2", NULL}; // Set the last but one element if you want to execute `ls` command on a particular file

    execv(args[0], args);
    printf("\n");
}

//execv is similar to execl, but instead of passing the arguments as separate parameters, you pass them as an array of strings (argv[]), where the last element must be NULL. 
//This is useful when you want to pass the arguments programmatically (e.g., from a pre-constructed array).

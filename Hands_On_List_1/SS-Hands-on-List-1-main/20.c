/*
============================================================================
Name : 20.c
Author : Nishad Bagade
Question : Find out the priority of your running program. 
           Modify the priority with nice command.

Date: 30th - Aug - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./20 5
Current priority: 0
New priority: 5
============================================================================
*/

#include <unistd.h> // Import for `nice` system call
#include <stdio.h>  // Import for `printf` function
#include <stdlib.h> // `atoi` conversion from string to int

void main(int argc, char *argv[])
{
    int priority, newp;
    if (argc != 2) //If the number of arguments (argc) is not equal to 2, 
                   //it prompts the user to provide the correct input.
        printf("Please pass the integer value \n");
    else
    {
        newp = atoi(argv[1]);
        priority = nice(0); // Get the priorty by adding 0 to current priorty
        printf("Current priority: %d\n", priority);
        priority = nice(newp); // Adds `newp` to the current priority
        printf("New priority: %d\n", priority);
    }
}

// The program is changing the nice value of the process in which the program itself is running. 
// This affects how the operating system prioritizes this process in terms of CPU scheduling.
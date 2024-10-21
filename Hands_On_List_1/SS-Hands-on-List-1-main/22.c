/*
============================================================================
Name : 22.c
Author : Nishad Bagade
Question : Write a program, open a file, call fork, and then write to the 
           file by both the child as well as the parent processes. 
           Check output of the file.
Date: 30th - Aug - 2024

Output: (Changes in file_1)
Hello, Mr Bruh, I am Nishad
Skibbidi Toilet, Brainrot, Level 7 Gyaat
W Rizz, L Rizz, Sigma Male 
Parent
Child

============================================================================
*/

#include <unistd.h> // import fork
#include <sys/types.h> // import open, fork
#include <sys/stat.h> // open
#include <fcntl.h> // open

void main() 
{
    char *filename = "/home/nishad/Desktop/Hand On List 1/file_1";
    int c_PID, fd;

    // Open the file in append mode. Create it if it doesn't exist.
    fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    c_PID = fork();

    if (c_PID != 0) // Parent
    {
        write(fd,"Parent\n!", 7);
    }
    else // Child
    {
        write(fd, "Child\n", 6);
    }
    close(fd);    
}
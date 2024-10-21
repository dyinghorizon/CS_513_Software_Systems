/*
============================================================================
Name : 8.c
Author : Nishad Bagade
Question : Write a program to open a file in read only mode, read line by 
           line and display each line as it is read. Close the file when end 
           of file is reached.
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./8 file_1
Hello, Mr Bruh, I am Nishad
Skibbidi Toilet, Brainrot, Level 7 Gyaat
W Rizz, L Rizz, Sigma Male 
Parent
Child
============================================================================
*/

#include <sys/types.h>  // Import for the `open` system call and type definitions like `mode_t`.
#include <sys/stat.h>   // Import for the `open` system call which uses flags like `O_RDONLY`.
#include <fcntl.h>      // Import for file control options, such as `O_RDONLY`.
#include <unistd.h>     // Import for `read`, `write`, and `close` system calls.
#include <stdio.h>      // Import for standard I/O functions like `printf` and `perror`.

void main(int argc, char *argv[]) {
    char *filename;      // Pointer to hold the file name provided as the argument.
    int fileDescriptor;  
    char buffer[1];      // Buffer of size 1 to read one character at a time.

    if (argc != 2) {
        // If not, print an error message.
        printf("Pass the file to be read as the argument\n");
    } 
    else {
        filename = argv[1];  
        fileDescriptor = open(filename, O_RDONLY);

        if (fileDescriptor == -1) {
            perror("Error while opening the file!");
        } 
        else {
            while (read(fileDescriptor, buffer, 1) == 1) {
                // Check if the character read is a newline (`\n`).
                if (buffer[0] == '\n') {
                    // If it's a newline, write a newline to the terminal.
                    write(STDOUT_FILENO, "\n", 1);  
                } else {
                    // Otherwise, write the character read from the file to the terminal.
                    write(STDOUT_FILENO, buffer, 1);
                }
            }
            
            close(fileDescriptor);
        }
    }
}

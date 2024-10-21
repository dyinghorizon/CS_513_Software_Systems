/*
============================================================================
Name : 6.c
Author : Nishad Bagade
Question : Write a program to take input from STDIN and display on STDOUT. 
		   Use only read/write system calls
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./6
Hello Nishad
Hello Nishad
============================================================================
*/

#include <unistd.h> // For read and write system calls

int main() {
    char buffer[1024];
    ssize_t bytesRead;

    // Read input from STDIN (file descriptor 0) into the buffer
    bytesRead = read(0, buffer, sizeof(buffer));

    // Check if the read was successful
    if (bytesRead > 0) {
        // Write the content of the buffer to STDOUT (file descriptor 1)
        write(1, buffer, bytesRead);
    }

    return 0;
}

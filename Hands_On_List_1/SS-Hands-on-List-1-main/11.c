/*
============================================================================
Name : 11.c
Author : Nishad Bagade
Question : Write a program to open a file, duplicate the file descriptor and append the file with both the
           descriptors and check whether the file is updated properly or not.
            a. use dup
            b. use dup2
            c. use fcntl
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./11
File content after append operations:
Hello I am Nishad!Original FD: Writing using fd
Dup FD: Writing using fd_dup


File content after append operations:
Hello I am Nishad!Original FD: Writing using fd
Dup FD: Writing using fd_dup
Dup2 FD: Writing using fd_dup2


File content after append operations:
Hello I am Nishad!Original FD: Writing using fd
Dup FD: Writing using fd_dup
Dup2 FD: Writing using fd_dup2
Fcntl FD: Writing using fd_fcntl
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void check_file_content(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    printf("File content after append operations:\n");
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
    printf("\n\n");
}

int main() {
    const char *filename = "testfile.txt";
    int fd, fd_dup, fd_dup2, fd_fcntl;

    // Open the file in append mode
    fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Duplicate the file descriptor using dup()
    fd_dup = dup(fd);
    if (fd_dup == -1) {
        perror("Error duplicating file descriptor with dup");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Append to the file using the original and duplicated file descriptors
    write(fd, "Original FD: Writing using fd\n", 30);
    write(fd_dup, "Dup FD: Writing using fd_dup\n", 30);

    // Check file content
    check_file_content(filename);

    // Duplicate the file descriptor using dup2()
    fd_dup2 = dup2(fd, fd_dup + 1);  // Choosing a different file descriptor
    if (fd_dup2 == -1) {
        perror("Error duplicating file descriptor with dup2");
        close(fd);
        close(fd_dup);
        exit(EXIT_FAILURE);
    }

    // Append to the file using the duplicated file descriptor
    write(fd_dup2, "Dup2 FD: Writing using fd_dup2\n", 31);

    check_file_content(filename);

    // Duplicate the file descriptor using fcntl()
    fd_fcntl = fcntl(fd, F_DUPFD, fd_dup2 + 1);  // Choosing yet another different file descriptor
    if (fd_fcntl == -1) {
        perror("Error duplicating file descriptor with fcntl");
        close(fd);
        close(fd_dup);
        close(fd_dup2);
        exit(EXIT_FAILURE);
    }

    // Append to the file using the fcntl-duplicated file descriptor
    write(fd_fcntl, "Fcntl FD: Writing using fd_fcntl\n", 33);

    check_file_content(filename);

    close(fd);
    close(fd_dup);
    close(fd_dup2);
    close(fd_fcntl);

    return 0;
}

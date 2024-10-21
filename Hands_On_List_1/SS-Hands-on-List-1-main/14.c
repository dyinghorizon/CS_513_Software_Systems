/*
============================================================================
Name : 14.c
Author : Nishad Bagade
Question : Write a program to find the type of a file.
            a. Input should be taken from command line.
            b. program should be able to identify any type of a file.
Date: 14th - Aug - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./14 /home/nishad/Desktop/Hand_On_List_1/f1/f2/hardlink.txt
File: /home/nishad/Desktop/Hand_On_List_1/f1/f2/hardlink.txt
Type: Regular file
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./14 /home/nishad/Desktop/Hand_On_List_1/f1/f2/myfifo
File: /home/nishad/Desktop/Hand_On_List_1/f1/f2/myfifo
Type: FIFO (named pipe)
============================================================================
*/

#include <stdio.h> // printf, fprintf
#include <sys/stat.h> // Import for the `stat` structure and macros for checking file types
#include <stdlib.h> // Import for standard library functions like exit and EXIT_FAILURE

void print_file_type(struct stat fileStat) {
    if (S_ISREG(fileStat.st_mode)) { // S_ISREG checks if it's a regular file
        printf("Regular file\n");
    } else if (S_ISDIR(fileStat.st_mode)) { // S_ISDIR checks if it's a directory
        printf("Directory\n");
    } else if (S_ISLNK(fileStat.st_mode)) { // S_ISLNK checks if it's a symbolic link
        printf("Symbolic link\n");
    } else if (S_ISCHR(fileStat.st_mode)) { // S_ISCHR checks if it's a character device file (used for devices like terminals)
        printf("Character device\n");
    } else if (S_ISBLK(fileStat.st_mode)) { // S_ISBLK checks if it's a block device file (used for devices like hard drives)
        printf("Block device\n");
    } else if (S_ISFIFO(fileStat.st_mode)) { // S_ISFIFO checks if it's a FIFO (named pipe)
        printf("FIFO (named pipe)\n");
    } else if (S_ISSOCK(fileStat.st_mode)) { // S_ISSOCK checks if it's a socket (used for inter-process communication)
        printf("Socket\n");
    } else {
        printf("Unknown file type\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct stat fileStat;
    if (stat(argv[1], &fileStat) < 0) {
        perror("stat");
        return EXIT_FAILURE;
    }

    printf("File: %s\n", argv[1]);
    printf("Type: ");
    print_file_type(fileStat);

    return EXIT_SUCCESS;
}

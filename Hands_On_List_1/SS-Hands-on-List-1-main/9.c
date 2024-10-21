/*
============================================================================
Name : 9.c
Author : Nishad Bagade
Question : Write a program to print the following information about a given file.
            a. inode
            b. number of hard links
            c. uid
            d. gid
            e. size
            f. block size
            g. number of blocks
            h. time of last access
            i. time of last modification
            j. time of last change
Date: 14th - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ ./9 file_1
File: file_1
Inode: 3416952
Number of hard links: 2
UID: 1000
GID: 1000
Size: 111 bytes
Block size: 4096 bytes
Number of blocks: 8
Time of last access: Sat Aug 31 18:27:58 2024
Time of last modification: Fri Aug 30 14:59:44 2024
Time of last change: Sat Aug 31 14:58:00 2024
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
    struct stat fileStat;

    // Check if the filename is provided
    if (argc != 2) {
        printf("Usage: ./fileinfo <filename>\n");
        exit(1);
    }

    // Retrieve the file information using stat
    if (stat(argv[1], &fileStat) < 0) {
        perror("Error retrieving file information");
        exit(1);
    }

    
    printf("File: %s\n", argv[1]);
    printf("Inode: %ld\n", fileStat.st_ino);
    printf("Number of hard links: %ld\n", fileStat.st_nlink);
    printf("UID: %d\n", fileStat.st_uid);
    printf("GID: %d\n", fileStat.st_gid);
    printf("Size: %ld bytes\n", fileStat.st_size);
    printf("Block size: %ld bytes\n", fileStat.st_blksize);
    printf("Number of blocks: %ld\n", fileStat.st_blocks);
    printf("Time of last access: %s", ctime(&fileStat.st_atime));
    printf("Time of last modification: %s", ctime(&fileStat.st_mtime));
    printf("Time of last change: %s", ctime(&fileStat.st_ctime));

    return 0;
}

/*
============================================================================
Name : 4.c
Author : Nishad Bagade
Question : Write a program to open an existing file with read write mode. 
		   Try O_EXCL flag also.
Date: 10th - Aug - 2024

Output: 
 open() system call executed successfully lol, the file descriptor of opened file is (3)

============================================================================
*/

# include <stdio.h>//printf
# include <fcntl.h>//open
# include <errno.h>//errno

void main() {
	int fd;
	fd = open("file_1", O_RDWR |  O_EXCL);
	if (fd == -1){
		printf("bruh - errno = (%d)\n", errno);
		perror("ERROR:");
	}
	else
	{
		printf("\n open() system call executed successfully lol, the file descriptor of opened file is (%d)\n", fd);
	}
}

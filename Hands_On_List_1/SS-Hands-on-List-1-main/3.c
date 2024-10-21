/*
============================================================================
Name : 3.c
Author : Nishad Bagade
Question : Write a program to create a file and print the file descriptor value. 
		   Use creat ( ) system call
Date: 10th - Aug - 2024

Output: 
creat system call executed successfully, the file descriptor of created file is 3

============================================================================
*/

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>

void main() {
	int fd;
	fd = creat("demo", O_RDWR);
	if (fd == -1) {
		printf("bruh -errno = (%d)\n", errno);
		perror("ERROR:");
	}
	else {
		printf("\n creat system call executed successfully, the file descriptor of created file is %d\n", fd);
	}
}

/*
============================================================================
Name : 21.c
Author : Nishad Bagade
Question : Write a program, call fork and print the parent and child 
	          process id.
Date: 28th - Aug - 2024

Output:
My Child Process id:47569
I am parent having id is:47568
Child Process having id:47569
My parent id is:47568

============================================================================
*/
# include <stdio.h> // Provides the printf()
# include <sys/types.h> // Defines data types used in system calls.
# include <unistd.h> // fork(), getpid(), and getppid() functions.

int main() {
	pid_t q;
	q = fork(); // pid_t is a data type defined in sys/types.h used for process IDs
	if(q==0)// child process
	{
		printf("Child Process having id:%d\n", getpid());
		printf("My parent id is:%d\n",getppid());
	}
	else{// parent process
		printf("My Child Process id:%d\n", q);
                printf("I am parent having id is:%d\n",getpid());
	}
}

/*
fork() returns:

    0 in the child process.
    The child's process ID in the parent process.
    A negative value if the fork fails.
*/

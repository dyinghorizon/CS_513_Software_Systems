/*
============================================================================
Name : 24.c
Author : Nishad Bagade
Question : Write a program to create an orphan process
Date: 28th - Aug - 2024

Output:
My Child Process id: 49529
I am parent having id is: 49528
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand On List 1$ Child Process having PID: 49529
My parent PID is: 2561

============================================================================
*/

# include <stdio.h> // printf
# include <sys/types.h> // pid_t
# include <unistd.h> // fork

int main() {
        pid_t q;
        q = fork();
        if(q==0)// child
        {
		sleep(2);
                printf("Child Process having PID: %d\n", getpid());
                printf("My parent PID is: %d\n",getppid());
        }
        else{// parent
                printf("My Child Process id: %d\n", q);
                printf("I am parent having id is: %d\n",getpid());
        }
}

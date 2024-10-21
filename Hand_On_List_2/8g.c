/*
============================================================================
Name : 8g.c
Author : Nishad Bagade
Question : Write a separate program using signal system call to catch the following signals.
                g. SIGPROF (use setitimer system call)
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./8g
Signal SIGPROF has been caught!
============================================================================
*/

#include <sys/time.h> // Import `setitimer`
#include <signal.h>   // Import for `signal`
#include <unistd.h>   // Import for `_exit`
#include <stdio.h>    // Import `perror` & `printf`

void callback()
{
    printf("Signal SIGPROF has been caught!\n");
    _exit(0);
}

void main()
{
    int timerStatus;                  // Determines success of `setitimer` call
    __sighandler_t signalStatus; // Determines status of `signal` call

    struct itimerval timer;

    // Generate signal after 1s
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    timerStatus = setitimer(ITIMER_PROF, &timer, 0);
    if (timerStatus == -1)
        perror("Error while setting an interval timer!");

    // Catch the SIGALRM signal
    signalStatus = signal(SIGPROF, (void *)callback);
    if (signalStatus == SIG_ERR)
        perror("Error while catching signal!");
    else
        while(1);
}
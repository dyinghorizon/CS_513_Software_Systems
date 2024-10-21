/*
============================================================================
Name : 19.c
Author : Nishad Bagade
Question : Write a program to find out time taken to execute getpid system call. 
           Use time stamp counter.
Date: 30th - Aug - 2024

Output:
PID: 9718
Time taken : 25113 
============================================================================
*/

#include <sys/types.h> //getpid()
#include <unistd.h>    //getpid()
#include <stdio.h>     //printf

// The following part handles the import of the `__rdtsc()` function based on the compiler being used.
#ifdef _MSC_VER
#include <intrin.h> // Include for __rdtsc() on Microsoft compilers (MSVC)
#else
#include <x86intrin.h> // Include for __rdtsc() on GCC or other compilers
#endif

// The `__rdtsc()` function is a processor instruction that reads the Time Stamp Counter (TSC),
// which counts the number of CPU cycles since the last reset, and provides a way to measure
// very fine-grained timings.


void main()
{
    unsigned int start, end, pid;

    start = __rdtsc(); //Capturing TSC value before getpid()
    pid = getpid();
    end = __rdtsc(); //Capturing TSC value after getpid()

    printf("PID: %d\n", pid);
    printf("Time taken : %u \n", (end - start));
}

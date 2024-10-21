/*
============================================================================
Name : 5.c
Author : Nishad Bagade
Question : Write a program to print the system limitation of
            a. maximum length of the arguments to the exec family of functions.
            b. maximum number of simultaneous process per user id.
            c. number of clock ticks (jiffy) per second.
            d. maximum number of open files
            e. size of a page
            f. total number of pages in the physical memory
            g. number of currently available pages in the physical memory.

Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./5
a. Maximum length of arguments to the exec family: 2097152 bytes
b. Maximum number of simultaneous processes per user ID: 61275
c. Number of clock ticks (jiffy) per second: 100
d. Maximum number of open files: 1048576
e. Size of a page: 4096 bytes
f. Total number of pages in physical memory: 3939429
g. Number of currently available pages in physical memory: 1394640
============================================================================
*/

#include <stdio.h>      // For printf
#include <unistd.h>     // For sysconf
#include <limits.h>     // For various system limits
#include <sys/sysinfo.h> // For sysinfo structure to get memory details

int main() {
    // a. Maximum length of the arguments to the exec family of functions.
    long arg_max = sysconf(_SC_ARG_MAX);
    if (arg_max != -1) {
        printf("a. Maximum length of arguments to the exec family: %ld bytes\n", arg_max);
    } else {
        perror("sysconf(_SC_ARG_MAX)");
    }

    // b. Maximum number of simultaneous processes per user ID.
    long max_procs = sysconf(_SC_CHILD_MAX);
    if (max_procs != -1) {
        printf("b. Maximum number of simultaneous processes per user ID: %ld\n", max_procs);
    } else {
        perror("sysconf(_SC_CHILD_MAX)");
    }

    // c. Number of clock ticks (jiffy) per second.
    long ticks_per_sec = sysconf(_SC_CLK_TCK);
    if (ticks_per_sec != -1) {
        printf("c. Number of clock ticks (jiffy) per second: %ld\n", ticks_per_sec);
    } else {
        perror("sysconf(_SC_CLK_TCK)");
    }

    // d. Maximum number of open files.
    long open_max = sysconf(_SC_OPEN_MAX);
    if (open_max != -1) {
        printf("d. Maximum number of open files: %ld\n", open_max);
    } else {
        perror("sysconf(_SC_OPEN_MAX)");
    }

    // e. Size of a page in bytes.
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size != -1) {
        printf("e. Size of a page: %ld bytes\n", page_size);
    } else {
        perror("sysconf(_SC_PAGESIZE)");
    }

    // f. Total number of pages in the physical memory.
    long total_pages = sysconf(_SC_PHYS_PAGES);
    if (total_pages != -1) {
        printf("f. Total number of pages in physical memory: %ld\n", total_pages);
    } else {
        perror("sysconf(_SC_PHYS_PAGES)");
    }

    // g. Number of currently available pages in the physical memory.
    long avail_pages = sysconf(_SC_AVPHYS_PAGES);
    if (avail_pages != -1) {
        printf("g. Number of currently available pages in physical memory: %ld\n", avail_pages);
    } else {
        perror("sysconf(_SC_AVPHYS_PAGES)");
    }

    return 0;
}

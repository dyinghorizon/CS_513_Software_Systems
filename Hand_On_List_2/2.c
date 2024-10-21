/*
============================================================================
Name : 2.c
Author : Nishad Bagade
Question : Write a program to print the system resource limits. Use getrlimit system call.
Date: 18th - Sept - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./2
CPU time limit:
  Soft limit: -1
  Hard limit: -1

File size limit:
  Soft limit: -1
  Hard limit: -1

Data segment size limit:
  Soft limit: -1
  Hard limit: -1

Stack size limit:
  Soft limit: 8388608
  Hard limit: -1

Core file size limit:
  Soft limit: 0
  Hard limit: -1

Resident set size limit:
  Soft limit: -1
  Hard limit: -1

Number of processes limit:
  Soft limit: 61275
  Hard limit: 61275

Number of open files limit:
  Soft limit: 1048576
  Hard limit: 1048576

Locked-in-memory address space limit:
  Soft limit: 2016985088
  Hard limit: 2016985088

Address space limit:
  Soft limit: -1
  Hard limit: -1
============================================================================
*/


#include <stdio.h>
#include <sys/resource.h>

void print_limit(int resource, const char *resource_name) {
    struct rlimit limit;
    
    // Get the resource limit using getrlimit
    if (getrlimit(resource, &limit) == 0) {
        printf("%s:\n", resource_name);
        printf("  Soft limit: %ld\n", (long) limit.rlim_cur);
        printf("  Hard limit: %ld\n\n", (long) limit.rlim_max);
    } else {
        perror("getrlimit");
    }
}

int main() {
    // Print resource limits for various system resources
    print_limit(RLIMIT_CPU, "CPU time limit");
    print_limit(RLIMIT_FSIZE, "File size limit");
    print_limit(RLIMIT_DATA, "Data segment size limit");
    print_limit(RLIMIT_STACK, "Stack size limit");
    print_limit(RLIMIT_CORE, "Core file size limit");
    print_limit(RLIMIT_RSS, "Resident set size limit");
    print_limit(RLIMIT_NPROC, "Number of processes limit");
    print_limit(RLIMIT_NOFILE, "Number of open files limit");
    print_limit(RLIMIT_MEMLOCK, "Locked-in-memory address space limit");
    print_limit(RLIMIT_AS, "Address space limit");

    return 0;
}

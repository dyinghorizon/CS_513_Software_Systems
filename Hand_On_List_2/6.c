/*
============================================================================
Name : 6.c
Author : Nishad Bagade
Question : Write a simple program to create three threads.
Date: 18th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./6
Thread 1: I am running
Thread 2: I am running
Thread 3: I am running
All threads have completed execution.
============================================================================
*/

#include <pthread.h>   // Import for pthread functions
#include <stdio.h>     // Import for printf
#include <stdlib.h>    // Import for exit
#include <unistd.h>    // Import for sleep

// Function to be executed by each thread
void* thread_function(void* arg) {
    int thread_num = *((int*)arg);  // Get the thread number from the argument
    printf("Thread %d: I am running\n", thread_num);
    pthread_exit(NULL);  // Exit the thread
}

int main() {
    pthread_t threads[3];  // Array to hold three thread identifiers
    int thread_args[3];    // Array to hold the arguments for each thread
    int result_code;
    int i;

    // Create three threads
    for(i = 0; i < 3; i++) {
        thread_args[i] = i + 1;  // Assign thread number (1, 2, 3)
        result_code = pthread_create(&threads[i], NULL, thread_function, (void*)&thread_args[i]);

        // Check if thread creation was successful
        if (result_code) {
            printf("Error creating thread %d: %d\n", i+1, result_code);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for each thread to finish execution
    for(i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have completed execution.\n");

    return 0;
}

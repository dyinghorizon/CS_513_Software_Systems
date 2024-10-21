/*
============================================================================
Name : 30.c
Author : Nishad Bagade
Description: Write a program to run a script at a specific time using a Daemon process.
Date: 31nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ps aux | grep run_script_daemon
nishad    108122  0.0  0.0   6632  2176 pts/1    S+   23:09   0:00 grep --color=auto run_script_daemon
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void create_daemon() {
    pid_t pid;

    // Fork the process
    pid = fork();

    // If fork failed, exit
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // If this is the parent process, exit
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Set file permissions mask
    umask(0);

    // Create a new session
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory to root
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Open a log file in write mode
    open("/tmp/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0600);
}

void run_script_at_time(int hour, int minute, const char *script_path) {
    while (1) {
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);

        // Check if the current time matches the specified time
        if (current_time->tm_hour == hour && current_time->tm_min == minute) {
            // Execute the script
            system(script_path);

            // Log the execution
            FILE *log = fopen("/tmp/daemon.log", "a");
            if (log) {
                fprintf(log, "Script executed at: %02d:%02d\n", hour, minute);
                fclose(log);
            }

            // Sleep for a minute to avoid multiple executions within the same minute
            sleep(60);
        }

        // Sleep for 1 second before checking the time again
        sleep(1);
    }
}

int main() {
    const char *script_path = "/path/to/your/script.sh";  // Path to your script
    int target_hour = 14;  // Target hour (24-hour format)
    int target_minute = 30;  // Target minute

    // Create the daemon process
    create_daemon();

    // Run the script at the specified time
    run_script_at_time(target_hour, target_minute, script_path);

    return 0;
}

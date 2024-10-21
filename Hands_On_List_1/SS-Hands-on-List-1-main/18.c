/*
============================================================================
Name : 18.c
Author : Nishad Bagade
Description: Creating and Initializing the Records
Date: 24nd - Aug - 2024

Output: 
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18
Initialized records in the file.
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18a
Modified record 2 to new value: 200
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hand_On_List_1$ ./18b
Read record 2: value = 200
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "records.dat";
    int fd;
    int records[] = {100, 150, 175};  // Three records

    // Open the file (create if it doesn't exist) and truncate it
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write the records to the file
    if (write(fd, records, sizeof(records)) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Initialized records in the file.\n");

    // Close the file
    close(fd);

    return 0;
}

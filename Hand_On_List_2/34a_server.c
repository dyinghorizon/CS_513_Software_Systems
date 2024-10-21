/*
============================================================================
Name : 34a_server.c
Author : Nishad Bagade
Question : Write a program to create a concurrent server.
            a. use fork
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./34a_server
Server is listening on port 8080...
New client connected
Client says: Hello from client
Connection closed with client
New client connected
Client says: Hello from client
Connection closed with client
^C
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Function to handle client communication
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    // Read message from the client
    valread = read(client_socket, buffer, BUFFER_SIZE);
    printf("Client says: %s\n", buffer);

    // Send response back to the client
    char *response = "Message received by server";
    send(client_socket, response, strlen(response), 0);

    // Close the client socket
    close(client_socket);
    printf("Connection closed with client\n");
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set the server address properties
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the server address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for client connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Accept a new client connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected\n");

        // Fork a child process to handle the new client
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
            continue;
        }

        if (pid == 0) {
            // In the child process
            close(server_fd);  // Close the listening socket in the child
            handle_client(client_socket);  // Handle client communication
            exit(0);  // Child process exits after handling the client
        } else {
            // In the parent process
            close(client_socket);  // Parent closes the connected socket
            waitpid(-1, NULL, WNOHANG);  // Reap zombie processes
        }
    }

    return 0;
}

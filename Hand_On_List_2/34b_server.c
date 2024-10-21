/*
============================================================================
Name : 34b_server.c
Author : Nishad Bagade
Question : Write a program to create a concurrent server.
            b. use pthread_create
Date: 20th - Sept - 2024

Output:
nishad@nishad-ROG-Zephyrus-G14-GA401QM-GA401QM:~/Desktop/Hands_On_List_2$ ./34b_server
Server is listening on port 8080...
New client connected
Handler assigned for client
Client says: Hello from client
Connection closed with client
New client connected
Handler assigned for client
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
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Function to handle communication with the client
void* handle_client(void* client_socket) {
    int socket = *(int*)client_socket;
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    // Read message from the client
    valread = read(socket, buffer, BUFFER_SIZE);
    printf("Client says: %s\n", buffer);

    // Send response back to the client
    char *response = "Message received by server";
    send(socket, response, strlen(response), 0);

    // Close the client socket
    close(socket);
    printf("Connection closed with client\n");
    
    // Free the memory allocated for the client socket
    free(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address properties
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the server address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
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
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected\n");

        // Allocate memory for the client socket to pass to the thread
        new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
        } else {
            printf("Handler assigned for client\n");
        }

        // Detach the thread to automatically reclaim resources when it's done
        pthread_detach(thread_id);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "./functions/admin.h"
#include "./functions/customer.h"
#include "./functions/manager.h"
#include "./functions/employee.h"

#define PORT 8080

void connection_handler(int connectionFileDescriptor);

int main() {
    int serverFileDescriptor, connectionFileDescriptor;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Create socket
    serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFileDescriptor == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverFileDescriptor, 5) == -1) {
        perror("Error listening for connections");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept connection
        connectionFileDescriptor = accept(serverFileDescriptor, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (connectionFileDescriptor == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Connection established with client\n");

        // Handle connection
        connection_handler(connectionFileDescriptor);

        close(connectionFileDescriptor);
        printf("Connection closed with client\n");
    }

    close(serverFileDescriptor);
    return 0;
}

void connection_handler(int connectionFileDescriptor) {
    char buffer[1024];
    ssize_t bytesRead, bytesWritten;

    // Send initial prompt
    const char *initialPrompt = "Welcome to Spooks bank!\nWho are you?\n1. Admin\n2. Customer\n3. Manager\n4. Employee\nPress any other number to exit\nEnter the number corresponding to the choice!";
    bytesWritten = write(connectionFileDescriptor, initialPrompt, strlen(initialPrompt));
    if (bytesWritten == -1) {
        perror("Error writing initial prompt to client");
        return;
    }

    // Read user choice
    bytesRead = read(connectionFileDescriptor, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Error reading user choice from client");
        return;
    }
    buffer[bytesRead] = '\0';

    int choice = atoi(buffer);
    switch (choice) {
        case 1:
            // Admin
            admin_operation_handler(connectionFileDescriptor);
            break;
        case 2:
            // Customer
            customer_operation_handler(connectionFileDescriptor);
            break;
        case 3:
            // Manager
            manager_operation_handler(connectionFileDescriptor);
            break;
        case 4:
            // Employee
            employee_operation_handler(connectionFileDescriptor);
            break;
        default:
            // Exit
            break;
    }

    printf("Terminating connection to client!\n");
}
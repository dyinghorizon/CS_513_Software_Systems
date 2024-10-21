#ifndef EMPLOYEE_FUNCTIONS
#define EMPLOYEE_FUNCTIONS

#include "./common.h"
#include "../record-struct/loan.h"

// Define the loggedInEmployee variable
struct Employee loggedInEmployee;

// Function Prototypes =================================

bool employee_operation_handler(int connFD);
bool deposit_loan_amount(int customerID, long int amount);

// bool change_password(int connFD);
bool accept_reject_loan_application(int connFD);
bool add_new_customer(int connFD);
bool modify_customer_details(int connFD);
bool employee_manager_login_handler(int connFD, struct Employee *ptrToEmployee, bool isManager);
bool view_assigned_loan_applications(int connFD);

// =====================================================

// Add this line at the top of the file
struct Employee loggedInEmployee;

// Function Definition =================================

bool employee_operation_handler(int connFD)
{
    struct Employee loggedInEmployee;
    if (employee_manager_login_handler(connFD, &loggedInEmployee, false)) // Employee login
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Employee login successful!\n");
        while (1)
        {
            strcat(writeBuffer, "1. Approve/Reject Loans\n");
            strcat(writeBuffer, "2. View Assigned Loan Applications\n");
            strcat(writeBuffer, "3. Add New Customer\n");
            strcat(writeBuffer, "4. Modify Customer Details\n");
            strcat(writeBuffer, "5. Change Password\n");
            strcat(writeBuffer, "6. Logout\n");
            strcat(writeBuffer, "Enter your choice: ");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing EMPLOYEE_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for EMPLOYEE_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                // Approve/Reject Loans
                if (!accept_reject_loan_application(connFD))
                {
                    perror("Error while processing loan application!");
                }
                break;
            case 2:
                // View Assigned Loan Applications
                if (!view_assigned_loan_applications(connFD))
                {
                    perror("Error while viewing assigned loan applications!");
                }
                break;
            case 3:
                // Add New Customer
                if (!add_new_customer(connFD))
                {
                    perror("Error while adding new customer!");
                }
                break;
            case 4:
                // Modify Customer Details
                if (!modify_customer_details(connFD))
                {
                    perror("Error while modifying customer details!");
                }
                break;
            case 5:
                // Change Password
                if (!change_password(connFD))
                {
                    perror("Error while changing password!");
                }
                break;
            case 6:
                // Logout
                writeBytes = write(connFD, "Logging out...$TERM$", strlen("Logging out...$TERM$"));
                if (writeBytes == -1)
                {
                    perror("Error while writing logout message to client!");
                }
                return false;
            default:
                // Invalid choice
                writeBytes = write(connFD, "Invalid choice! Logging out...$TERM$", strlen("Invalid choice! Logging out...$TERM$"));
                if (writeBytes == -1)
                {
                    perror("Error while writing invalid choice message to client!");
                }
                return false;
            }
        }
    }
    else
    {
        // EMPLOYEE LOGIN FAILED
        return false;
    }
}

bool view_assigned_loan_applications(int connFD)
{
    char writeBuffer[1000];
    ssize_t writeBytes;

    struct LoanApplication loanApplication;
    int employeeID = loggedInEmployee.id;

    // Open the loan file
    int loanFileDescriptor = open("records/loan.bank", O_RDONLY);
    if (loanFileDescriptor == -1)
    {
        perror("Error opening loan file!");
        return false;
    }

    // Read and display all assigned loan applications
    while (read(loanFileDescriptor, &loanApplication, sizeof(struct LoanApplication)) > 0)
    {
        if (loanApplication.assignedEmployeeID == employeeID)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Loan Application ID: %d\nCustomer ID: %d\nAmount: ₹ %ld\nStatus: %s\n\n",
                    loanApplication.loanApplicationID,
                    loanApplication.customerID,
                    loanApplication.amount,
                    (loanApplication.status == LOAN_PENDING ? "Pending" : (loanApplication.status == LOAN_ACCEPTED ? "Accepted" : "Rejected")));
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing loan application details to client!");
                close(loanFileDescriptor);
                return false;
            }
        }
    }

    close(loanFileDescriptor);

    // Inform client of end of list
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "End of assigned loan applications.\n");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing end of list message to client!");
        return false;
    }

    return true;
}

bool accept_reject_loan_application(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct LoanApplication loanApplication;

    // Prompt for loan application ID
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter the loan application ID: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing loan application ID prompt to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading loan application ID from client!");
        return false;
    }
    int loanApplicationID = atoi(readBuffer);

    // Fetch loan application details
    if (!get_loan_application_details(loanApplicationID, &loanApplication))
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Invalid loan application ID.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing invalid loan application ID message to client!");
        }
        return false;
    }

    // Prompt for acceptance or rejection
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Do you want to (a)ccept or (r)eject this loan application? (a/r): ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing acceptance/rejection prompt to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading acceptance/rejection decision from client!");
        return false;
    }

    // Update loan application status based on employee's decision
    if (readBuffer[0] == 'a' || readBuffer[0] == 'A')
    {
        loanApplication.status = LOAN_ACCEPTED;

        // Deposit loan amount to customer's account
        if (!deposit_loan_amount(loanApplication.customerID, loanApplication.amount))
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Error depositing loan amount to customer's account.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing error message to client!");
            }
            return false;
        }
    }
    else if (readBuffer[0] == 'r' || readBuffer[0] == 'R')
    {
        loanApplication.status = LOAN_REJECTED;
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Invalid choice! Operation aborted.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing invalid choice message to client!");
        }
        return false;
    }

    // Save updated loan application to file
    if (!update_loan_application(loanApplicationID, &loanApplication))
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Error updating loan application status.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing error message to client!");
        }
        return false;
    }

    // Inform client of successful operation
    bzero(writeBuffer, sizeof(writeBuffer));
    if (loanApplication.status == LOAN_ACCEPTED)
    {
        strcpy(writeBuffer, "Loan application accepted and amount deposited to customer's account.\n");
    }
    else
    {
        strcpy(writeBuffer, "Loan application rejected.\n");
    }
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }

    return true;
}

// bool change_password(int connFD)
// {
//     // Implement the function based on your requirements
//     return true;
// }

bool add_new_customer(int connFD)
{
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Customer newCustomer;

    // Get customer details from employee
    writeBytes = write(connFD, "Enter customer name: ", strlen("Enter customer name: "));
    if (writeBytes == -1)
    {
        perror("Error writing customer name prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name from client!");
        return false;
    }
    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, "Enter customer gender (M/F/O): ", strlen("Enter customer gender (M/F/O): "));
    if (writeBytes == -1)
    {
        perror("Error writing customer gender prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender from client!");
        return false;
    }
    newCustomer.gender = readBuffer[0];

    writeBytes = write(connFD, "Enter customer age: ", strlen("Enter customer age: "));
    if (writeBytes == -1)
    {
        perror("Error writing customer age prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age from client!");
        return false;
    }
    newCustomer.age = atoi(readBuffer);

    // Generate login ID and password
    sprintf(newCustomer.login, "%s-%d", newCustomer.name, newCustomer.id);
    strcpy(newCustomer.password, "defaultpassword");

    // Save new customer to file
    int customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY | O_APPEND);
    if (customerFileDescriptor == -1)
    {
        perror("Error opening customer file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(struct Customer));
    if (writeBytes == -1)
    {
        perror("Error writing new customer to file!");
        close(customerFileDescriptor);
        return false;
    }
    close(customerFileDescriptor);

    writeBytes = write(connFD, "New customer added successfully!\n", strlen("New customer added successfully!\n"));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }
    return true;
}

bool modify_customer_details(int connFD)
{
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    int customerID;
    struct Customer customer;

    // Get customer ID from employee
    writeBytes = write(connFD, "Enter customer ID to modify: ", strlen("Enter customer ID to modify: "));
    if (writeBytes == -1)
    {
        perror("Error writing customer ID prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer ID from client!");
        return false;
    }
    customerID = atoi(readBuffer);

    // Open customer file and find the customer record
    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
    if (customerFileDescriptor == -1)
    {
        perror("Error opening customer file!");
        return false;
    }
    off_t offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to customer record!");
        close(customerFileDescriptor);
        return false;
    }
    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining write lock on customer record!");
        close(customerFileDescriptor);
        return false;
    }
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1)
    {
        perror("Error reading customer record!");
        close(customerFileDescriptor);
        return false;
    }

    // Get new customer details from employee
    writeBytes = write(connFD, "Enter new customer name: ", strlen("Enter new customer name: "));
    if (writeBytes == -1)
    {
        perror("Error writing customer name prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name from client!");
        return false;
    }
    strcpy(customer.name, readBuffer);

    writeBytes = write(connFD, "Enter new customer gender (M/F/O): ", strlen("Enter new customer gender (M/F/O): "));
    if (writeBytes == -1)
    {
        perror("Error writing customer gender prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender from client!");
        return false;
    }
    customer.gender = readBuffer[0];

    writeBytes = write(connFD, "Enter new customer age: ", strlen("Enter new customer age: "));
    if (writeBytes == -1)
    {
        perror("Error writing customer age prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age from client!");
        return false;
    }
    customer.age = atoi(readBuffer);

    // Save modified customer to file
    lseek(customerFileDescriptor, offset, SEEK_SET);
    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (writeBytes == -1)
    {
        perror("Error writing modified customer to file!");
        close(customerFileDescriptor);
        return false;
    }
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);
    close(customerFileDescriptor);

    writeBytes = write(connFD, "Customer details modified successfully!\n", strlen("Customer details modified successfully!\n"));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }
    return true;
}

// =====================================================

#endif



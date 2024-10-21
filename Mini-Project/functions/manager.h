#ifndef MANAGER_FUNCTIONS
#define MANAGER_FUNCTIONS

#include "./common.h"
#include "../record-struct/employee.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // Include for open()

// Define missing identifiers
#define MANAGER_FILE "manager_file_path"
#define SETVAL 16

// Declare the missing variable
struct Manager {
    int id;
} loggedInManager;

int semIdentifier;

// Add this line at the top of the file
struct Employee loggedInEmployee;

// Function Prototypes =================================

bool manager_operation_handler(int connFD);
bool activate_deactivate_account(int connFD);
bool assign_loan_application(int connFD);
bool review_customer_feedback(int connFD);
bool change_password(int connFD);
bool login_handler(bool isAdmin, int connFD, struct Customer *ptrToCustomer); // Ensure consistency
bool employee_manager_login_handler(int connFD, struct Employee *ptrToEmployee, bool isManager);
bool get_loan_application_details(int loanApplicationID, struct LoanApplication *loanApplication);
bool get_employee_details(int employeeID, struct Employee *employee);
bool update_loan_application(int loanApplicationID, struct LoanApplication *loanApplication);

// =====================================================
// Function Definition =================================

bool manager_operation_handler(int connFD)
{
    struct Employee loggedInEmployee;
    if (employee_manager_login_handler(connFD, &loggedInEmployee, true)) // Manager login
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(MANAGER_FILE, loggedInEmployee.id); // Generate a key based on the manager ID hence, different managers will have different semaphores

        union semun
        {
            int val; // Value of the semaphore
        } semSet;

        int semctlStatus;
        semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
        if (semIdentifier == -1)
        {
            semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (semIdentifier == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; // Set a binary semaphore
            semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary semaphore!");
                _exit(1);
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Manager login successful!\n");
        while (1)
        {
            strcat(writeBuffer, "1. Activate/Deactivate Customer Accounts\n");
            strcat(writeBuffer, "2. Assign Loan Application Processes to Employees\n");
            strcat(writeBuffer, "3. Review Customer Feedback\n");
            strcat(writeBuffer, "4. Change Password\n");
            strcat(writeBuffer, "5. Logout\n");
            strcat(writeBuffer, "Enter your choice: ");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing MANAGER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for MANAGER_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                // Activate/Deactivate Customer Accounts
                activate_deactivate_account(connFD);
                break;
            case 2:
                // Assign Loan Application Processes to Employees
                assign_loan_application(connFD);
                break;
            case 3:
                // Review Customer Feedback
                review_customer_feedback(connFD);
                break;
            case 4:
                // Change Password
                change_password(connFD);
                break;
            case 5:
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
        // MANAGER LOGIN FAILED
        return false;
    }
}

bool activate_deactivate_account(int connFD)
{
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Account account;

    // Prompt for account number
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter the account number: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing account number prompt to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading account number from client!");
        return false;
    }
    int accountNumber = atoi(readBuffer);

    // Open the account file
    int accountFileDescriptor = open("records/account.bank", O_RDWR);
    if (accountFileDescriptor == -1)
    {
        perror("Error opening account file!");
        return false;
    }

    // Fetch account details
    off_t offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to account record!");
        close(accountFileDescriptor);
        return false;
    }

    readBytes = read(accountFileDescriptor, &account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error reading account record!");
        close(accountFileDescriptor);
        return false;
    }

    // Display account details to manager
    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Account Details - \n\tAccount Number: %d\n\tAccount Type: %s\n\tAccount Status: %s\n\tAccount Balance: ₹ %ld\n\tPrimary Owner ID: %d\n\tSecondary Owner ID: %d\n",
            account.accountNumber,
            (account.isRegularAccount ? "Regular" : "Joint"),
            (account.active ? "Active" : "Deactivated"),
            account.balance,
            account.owners[0],
            account.owners[1]);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing account details to client!");
        close(accountFileDescriptor);
        return false;
    }

    // Confirm manager's decision
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Do you want to (a)ctivate or (d)eactivate this account? (a/d): ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing activation/deactivation prompt to client!");
        close(accountFileDescriptor);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading activation/deactivation decision from client!");
        close(accountFileDescriptor);
        return false;
    }

    // Update account status based on manager's decision
    if (readBuffer[0] == 'a' || readBuffer[0] == 'A')
    {
        account.active = true;
    }
    else if (readBuffer[0] == 'd' || readBuffer[0] == 'D')
    {
        account.active = false;
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
        close(accountFileDescriptor);
        return false;
    }

    // Write updated account record back to file
    offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to account record for update!");
        close(accountFileDescriptor);
        return false;
    }

    writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
    if (writeBytes == -1)
    {
        perror("Error writing updated account record!");
        close(accountFileDescriptor);
        return false;
    }

    close(accountFileDescriptor);

    // Inform manager of successful operation
    bzero(writeBuffer, sizeof(writeBuffer));
    if (account.active)
    {
        strcpy(writeBuffer, "Account successfully activated.\n");
    }
    else
    {
        strcpy(writeBuffer, "Account successfully deactivated.\n");
    }
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }

    return true;
}

bool assign_loan_application(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct LoanApplication loanApplication;
    struct Employee employee;

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

    // Prompt for employee ID
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter the employee ID to assign the loan application: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing employee ID prompt to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading employee ID from client!");
        return false;
    }
    int employeeID = atoi(readBuffer);

    // Fetch employee details
    if (!get_employee_details(employeeID, &employee))
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Invalid employee ID.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing invalid employee ID message to client!");
        }
        return false;
    }

    // Assign loan application to employee
    loanApplication.assignedEmployeeID = employeeID;

    // Save updated loan application to file
    if (!update_loan_application(loanApplicationID, &loanApplication))
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Error assigning loan application to employee.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing error message to client!");
        }
        return false;
    }

    // Inform client of successful assignment
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Loan application assigned successfully!\n");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }

    return true;
}

bool review_customer_feedback(int connFD)
{
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    int feedbackFileDescriptor;

    // Open the feedback file
    feedbackFileDescriptor = open("records/feedback.txt", O_RDONLY);
    if (feedbackFileDescriptor == -1)
    {
        perror("Error opening feedback file!");
        writeBytes = write(connFD, "Error opening feedback file!\n", strlen("Error opening feedback file!\n"));
        return false;
    }

    // Read the feedback file and send it to the client
    while ((readBytes = read(feedbackFileDescriptor, readBuffer, sizeof(readBuffer) - 1)) > 0)
    {
        readBuffer[readBytes] = '\0'; // Null-terminate the buffer
        writeBytes = write(connFD, readBuffer, strlen(readBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing feedback to client!");
            close(feedbackFileDescriptor);
            return false;
        }
    }

    if (readBytes == -1)
    {
        perror("Error reading feedback file!");
        close(feedbackFileDescriptor);
        return false;
    }

    close(feedbackFileDescriptor);

    // Send a message indicating the end of the feedback
    writeBytes = write(connFD, "\nEnd of feedback.\n", strlen("\nEnd of feedback.\n"));
    if (writeBytes == -1)
    {
        perror("Error writing end of feedback message to client!");
        return false;
    }

    // Dummy read to wait for client acknowledgment
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading acknowledgment from client!");
        return false;
    }

    return true;
}
// =====================================================

#endif


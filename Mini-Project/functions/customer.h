#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS

// Semaphores are necessary joint account due the design choice I've made
#include <sys/ipc.h>
#include <sys/sem.h>

struct Customer loggedInCustomer;
int semIdentifier;

// Function Prototypes =================================

bool transfer_funds(int connFD);
bool customer_operation_handler(int connFD);
bool deposit(int connFD);
bool withdraw(int connFD);
bool get_balance(int connFD);
bool change_password(int connFD);
bool add_feedback(int connFD);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *sem_op);
void write_transaction_to_array(int *transactionArray, int ID);
int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation);
bool get_account_details_by_number(int accountNumber, struct Account *account);
bool apply_for_loan(int connFD);
bool view_loan_application(int connFD);

// =====================================================

// Function Definition =================================
bool get_account_details_by_number(int accountNumber, struct Account *account)
{
    int accountFileDescriptor = open("records/account.bank", O_RDONLY);
    if (accountFileDescriptor == -1)
    {
        perror("Error opening account file!");
        return false;
    }

    off_t offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to account record!");
        close(accountFileDescriptor);
        return false;
    }

    ssize_t readBytes = read(accountFileDescriptor, account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error reading account record!");
        close(accountFileDescriptor);
        return false;
    }
    close(accountFileDescriptor);
    return true;
}

bool apply_for_loan(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct LoanApplication loanApplication;
    loanApplication.customerID = loggedInCustomer.id;

    // Determine the next loan application ID
    int loanFileDescriptor = open("records/loan.bank", O_RDONLY);
    if (loanFileDescriptor == -1)
    {
        perror("Error opening loan file!");
        return false;
    }

    struct LoanApplication tempLoanApplication;
    int maxLoanID = 1000; // Start from 1001
    while (read(loanFileDescriptor, &tempLoanApplication, sizeof(struct LoanApplication)) > 0)
    {
        if (tempLoanApplication.loanApplicationID > maxLoanID)
        {
            maxLoanID = tempLoanApplication.loanApplicationID;
        }
    }
    close(loanFileDescriptor);

    loanApplication.loanApplicationID = maxLoanID + 1;

    // Prompt for loan amount
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter the loan amount: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing loan amount prompt to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading loan amount from client!");
        return false;
    }
    loanApplication.amount = atol(readBuffer);

    // Save loan application to file
    loanFileDescriptor = open("records/loan.bank", O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
    if (loanFileDescriptor == -1)
    {
        perror("Error opening loan file!");
        return false;
    }

    writeBytes = write(loanFileDescriptor, &loanApplication, sizeof(struct LoanApplication));
    if (writeBytes == -1)
    {
        perror("Error writing loan application to file!");
        close(loanFileDescriptor);
        return false;
    }
    close(loanFileDescriptor);

    // Inform client of successful loan application
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Loan application submitted successfully!\n");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }

    return true;
}

bool customer_operation_handler(int connFD)
{
    if (login_handler(false, connFD, &loggedInCustomer))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(CUSTOMER_FILE, loggedInCustomer.account); // Generate a key based on the account number hence, different customers will have different semaphores

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
        strcpy(writeBuffer, "Customer login successful!\n");
        while (1)
        {
            strcat(writeBuffer, "1. Get Customer Details\n");
            strcat(writeBuffer, "2. Deposit Money\n");
            strcat(writeBuffer, "3. Withdraw Money\n");
            strcat(writeBuffer, "4. Get Balance\n");
            strcat(writeBuffer, "5. Get Transaction Information\n");
            strcat(writeBuffer, "6. Change Password\n");
            strcat(writeBuffer, "7. Transfer Funds\n");
            strcat(writeBuffer, "8. Add Feedback\n");
            strcat(writeBuffer, "9. Apply for Loan\n");
            strcat(writeBuffer, "10. View Loan Applications\n");
            strcat(writeBuffer, "Press any other key to logout\n");
            strcat(writeBuffer, "Enter your choice: ");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing CUSTOMER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for CUSTOMER_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                get_customer_details(connFD, loggedInCustomer.id);
                break;
            case 2:
                deposit(connFD);
                break;
            case 3:
                withdraw(connFD);
                break;
            case 4:
                get_balance(connFD);
                break;
            case 5:
                get_transaction_details(connFD, loggedInCustomer.account);
                break;
            case 6:
                change_password(connFD);
                break;
            case 7:
                transfer_funds(connFD);
                break;
            case 8:
                add_feedback(connFD);
                break;
            case 9:
                apply_for_loan(connFD);
                break;
            case 10:
                view_loan_application(connFD);
                break;
            default:
                writeBytes = write(connFD, "Logging out...\n", strlen("Logging out...\n"));
                if (writeBytes == -1)
                {
                    perror("Error while writing logout message to client!");
                }
                return false;
            }
        }
    }
    else
    {
        // CUSTOMER LOGIN FAILED
        return false;
    }
}

bool deposit(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.accountNumber = loggedInCustomer.account;

    long int depositAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        if (!account.active)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Account is deactivated. Cannot perform transactions.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing deactivated account message to client!");
            }
            unlock_critical_section(&semOp);
            return false;
        }

        // Prompt for deposit amount
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Enter the amount to deposit: ");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing deposit amount prompt to client!");
            unlock_critical_section(&semOp);
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading deposit amount from client!");
            unlock_critical_section(&semOp);
            return false;
        }
        depositAmount = atol(readBuffer);

        // Update account balance
        long int oldBalance = account.balance;
        account.balance += depositAmount;

        // Write updated account record back to file
        int accountFileDescriptor = open("records/account.bank", O_RDWR);
        if (accountFileDescriptor == -1)
        {
            perror("Error opening account file!");
            unlock_critical_section(&semOp);
            return false;
        }

        off_t offset = lseek(accountFileDescriptor, account.accountNumber * sizeof(struct Account), SEEK_SET);
        if (offset == -1)
        {
            perror("Error seeking to account record for update!");
            close(accountFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
        if (writeBytes == -1)
        {
            perror("Error writing updated account record!");
            close(accountFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        close(accountFileDescriptor);

        // Log the transaction
        int transactionID = write_transaction_to_file(account.accountNumber, oldBalance, account.balance, true);
        write_transaction_to_array(account.transactions, transactionID);

        // Inform client of successful deposit
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "Deposit successful. New balance: ₹ %ld\n", account.balance);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing success message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
    }
    else
    {
        // Error while getting account details
        unlock_critical_section(&semOp);
        return false;
    }

    unlock_critical_section(&semOp);
    return true;
}

bool withdraw(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.accountNumber = loggedInCustomer.account;

    long int withdrawAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        if (!account.active)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Account is deactivated. Cannot perform transactions.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing deactivated account message to client!");
            }
            unlock_critical_section(&semOp);
            return false;
        }

        // Prompt for withdrawal amount
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Enter the amount to withdraw: ");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing withdrawal amount prompt to client!");
            unlock_critical_section(&semOp);
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading withdrawal amount from client!");
            unlock_critical_section(&semOp);
            return false;
        }
        withdrawAmount = atol(readBuffer);

        // Check if sufficient balance is available
        if (account.balance < withdrawAmount)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Insufficient balance. Cannot perform withdrawal.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing insufficient balance message to client!");
            }
            unlock_critical_section(&semOp);
            return false;
        }

        // Update account balance
        long int oldBalance = account.balance;
        account.balance -= withdrawAmount;

        // Write updated account record back to file
        int accountFileDescriptor = open("records/account.bank", O_RDWR);
        if (accountFileDescriptor == -1)
        {
            perror("Error opening account file!");
            unlock_critical_section(&semOp);
            return false;
        }

        off_t offset = lseek(accountFileDescriptor, account.accountNumber * sizeof(struct Account), SEEK_SET);
        if (offset == -1)
        {
            perror("Error seeking to account record for update!");
            close(accountFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
        if (writeBytes == -1)
        {
            perror("Error writing updated account record!");
            close(accountFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        close(accountFileDescriptor);

        // Log the transaction
        int transactionID = write_transaction_to_file(account.accountNumber, oldBalance, account.balance, false);
        write_transaction_to_array(account.transactions, transactionID);

        // Inform client of successful withdrawal
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "Withdrawal successful. New balance: ₹ %ld\n", account.balance);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing success message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
    }
    else
    {
        // Error while getting account details
        unlock_critical_section(&semOp);
        return false;
    }

    unlock_critical_section(&semOp);
    return true;
}

bool get_balance(int connFD)
{
    char buffer[1000];
    struct Account account;
    account.accountNumber = loggedInCustomer.account;
    if (get_account_details(connFD, &account))
    {
        bzero(buffer, sizeof(buffer));
        if (account.active)
        {
            sprintf(buffer, "You have ₹ %ld imaginary money in our bank!^", account.balance);
            write(connFD, buffer, strlen(buffer));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, buffer, sizeof(buffer)); // Dummy read
    }
    else
    {
        // ERROR while getting balance
        return false;
    }
}

//Transfer funds from one account to another
bool transfer_funds(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account sourceAccount, destinationAccount;
    sourceAccount.accountNumber = loggedInCustomer.account;

    long int transferAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &sourceAccount))
    {
        if (!sourceAccount.active)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Source account is deactivated. Cannot perform transactions.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing deactivated source account message to client!");
            }
            unlock_critical_section(&semOp);
            return false;
        }

        // Prompt for destination account number
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Enter the destination account number: ");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing destination account number prompt to client!");
            unlock_critical_section(&semOp);
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading destination account number from client!");
            unlock_critical_section(&semOp);
            return false;
        }
        int destinationAccountNumber = atoi(readBuffer);

        // Fetch destination account details
        if (get_account_details_by_number(destinationAccountNumber, &destinationAccount))
        {
            if (!destinationAccount.active)
            {
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, "Destination account is deactivated. Cannot perform transactions.\n");
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                if (writeBytes == -1)
                {
                    perror("Error writing deactivated destination account message to client!");
                }
                unlock_critical_section(&semOp);
                return false;
            }

            // Prompt for transfer amount
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Enter the amount to transfer: ");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing transfer amount prompt to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error reading transfer amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }
            transferAmount = atol(readBuffer);

            // Check if sufficient balance is available in source account
            if (sourceAccount.balance < transferAmount)
            {
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, "Insufficient balance in source account. Cannot perform transfer.\n");
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                if (writeBytes == -1)
                {
                    perror("Error writing insufficient balance message to client!");
                }
                unlock_critical_section(&semOp);
                return false;
            }

            // Update account balances
            long int oldSourceBalance = sourceAccount.balance;
            long int oldDestinationBalance = destinationAccount.balance;
            sourceAccount.balance -= transferAmount;
            destinationAccount.balance += transferAmount;

            // Write updated source account record back to file
            int accountFileDescriptor = open("records/account.bank", O_RDWR);
            if (accountFileDescriptor == -1)
            {
                perror("Error opening account file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(accountFileDescriptor, sourceAccount.accountNumber * sizeof(struct Account), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to source account record for update!");
                close(accountFileDescriptor);
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(accountFileDescriptor, &sourceAccount, sizeof(struct Account));
            if (writeBytes == -1)
            {
                perror("Error writing updated source account record!");
                close(accountFileDescriptor);
                unlock_critical_section(&semOp);
                return false;
            }

            // Write updated destination account record back to file
            offset = lseek(accountFileDescriptor, destinationAccount.accountNumber * sizeof(struct Account), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to destination account record for update!");
                close(accountFileDescriptor);
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(accountFileDescriptor, &destinationAccount, sizeof(struct Account));
            if (writeBytes == -1)
            {
                perror("Error writing updated destination account record!");
                close(accountFileDescriptor);
                unlock_critical_section(&semOp);
                return false;
            }

            close(accountFileDescriptor);

            // Log the transactions
            int sourceTransactionID = write_transaction_to_file(sourceAccount.accountNumber, oldSourceBalance, sourceAccount.balance, false);
            write_transaction_to_array(sourceAccount.transactions, sourceTransactionID);

            int destinationTransactionID = write_transaction_to_file(destinationAccount.accountNumber, oldDestinationBalance, destinationAccount.balance, true);
            write_transaction_to_array(destinationAccount.transactions, destinationTransactionID);

            // Inform client of successful transfer
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Transfer successful. New balance: ₹ %ld\n", sourceAccount.balance);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing success message to client!");
                unlock_critical_section(&semOp);
                return false;
            }
        }
        else
        {
            // Error while getting destination account details
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "Error fetching destination account details.\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing error message to client!");
            }
            unlock_critical_section(&semOp);
            return false;
        }
    }
    else
    {
        // Error while getting source account details
        unlock_critical_section(&semOp);
        return false;
    }

    unlock_critical_section(&semOp);
    return true;
}

bool change_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    // Lock the critical section
    struct sembuf semOp = {0, -1, SEM_UNDO};
    int semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        unlock_critical_section(&semOp);
        return false;
    }

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInCustomer.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(loggedInCustomer.password, newPassword);

            int customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
            if (customerFileDescriptor == -1)
            {
                perror("Error opening customer file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(customerFileDescriptor, loggedInCustomer.id * sizeof(struct Customer), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Customer));
            if (writeBytes == -1)
            {
                perror("Error storing updated customer password into customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

            close(customerFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            unlock_critical_section(&semOp);

            return true;
        }
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    unlock_critical_section(&semOp);

    return false;
}

bool add_feedback(int connFD)
{
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Feedback newFeedback;

    // Get feedback details from customer
    writeBytes = write(connFD, "Enter your feedback: ", strlen("Enter your feedback: "));
    if (writeBytes == -1)
    {
        perror("Error writing feedback prompt to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading feedback from client!");
        return false;
    }
    strcpy(newFeedback.feedbackText, readBuffer);
    newFeedback.customerID = loggedInCustomer.id;
    newFeedback.feedbackTime = time(NULL);

    // Save feedback to feedback.txt
    int feedbackFileDescriptor = open("records/feedback.txt", O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
    if (feedbackFileDescriptor == -1)
    {
        perror("Error opening feedback file!");
        return false;
    }

    // Write feedback to file
    snprintf(writeBuffer, sizeof(writeBuffer), "Customer ID: %d\nCustomer Name: %s\nFeedback: %s\nTime: %s\n\n",
             newFeedback.customerID, loggedInCustomer.name, newFeedback.feedbackText, ctime(&newFeedback.feedbackTime));
    writeBytes = write(feedbackFileDescriptor, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing feedback to file!");
        close(feedbackFileDescriptor);
        return false;
    }
    close(feedbackFileDescriptor);

    writeBytes = write(connFD, "Feedback added successfully!\n", strlen("Feedback added successfully!\n"));
    if (writeBytes == -1)
    {
        perror("Error writing success message to client!");
        return false;
    }
    return true;
}

bool lock_critical_section(struct sembuf *semOp)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp)
{
    semOp->sem_op = 1;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

void write_transaction_to_array(int *transactionArray, int ID)
{
    // Check if there's any free space in the array to write the new transaction ID
    int iter = 0;
    while (transactionArray[iter] != -1)
        iter++;

    if (iter >= MAX_TRANSACTIONS)
    {
        // No space
        for (iter = 1; iter < MAX_TRANSACTIONS; iter++)
            // Shift elements one step back discarding the oldest transaction
            transactionArray[iter - 1] = transactionArray[iter];
        transactionArray[iter - 1] = ID;
    }
    else
    {
        // Space available
        transactionArray[iter] = ID;
    }
}

int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation)
{
    struct Transaction newTransaction;
    newTransaction.accountNumber = accountNumber;
    newTransaction.oldBalance = oldBalance;
    newTransaction.newBalance = newBalance;
    newTransaction.operation = operation;
    newTransaction.transactionTime = time(NULL);

    ssize_t readBytes, writeBytes;

    int transactionFileDescriptor = open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);

    // Get most recent transaction number
    off_t offset = lseek(transactionFileDescriptor, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0)
    {
        // There exists at least one transaction record
        struct Transaction prevTransaction;
        readBytes = read(transactionFileDescriptor, &prevTransaction, sizeof(struct Transaction));

        newTransaction.transactionID = prevTransaction.transactionID + 1;
    }
    else
        // No transaction records exist
        newTransaction.transactionID = 0;

    writeBytes = write(transactionFileDescriptor, &newTransaction, sizeof(struct Transaction));

    return newTransaction.transactionID;
}

bool view_loan_application(int connFD)
{
    char writeBuffer[1000];
    ssize_t writeBytes;

    struct LoanApplication loanApplication;
    int customerID = loggedInCustomer.id;

    // Open the loan file
    int loanFileDescriptor = open("records/loan.bank", O_RDONLY);
    if (loanFileDescriptor == -1)
    {
        perror("Error opening loan file!");
        return false;
    }

    // Read and display all loan applications for the customer
    while (read(loanFileDescriptor, &loanApplication, sizeof(struct LoanApplication)) > 0)
    {
        if (loanApplication.customerID == customerID)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Loan Application ID: %d\nAmount: ₹ %ld\nStatus: %s\n\n",
                    loanApplication.loanApplicationID,
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
    strcpy(writeBuffer, "End of loan applications.\n");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing end of list message to client!");
        return false;
    }

    return true;
}
// =====================================================

#endif










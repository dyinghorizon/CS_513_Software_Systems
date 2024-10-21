#ifndef LOAN_RECORD
#define LOAN_RECORD

struct LoanApplication {
    int loanApplicationID;
    int customerID;
    int assignedEmployeeID;
    long int amount;
    int status; // 0: Pending, 1: Accepted, 2: Rejected
};

#define LOAN_PENDING 0
#define LOAN_ACCEPTED 1
#define LOAN_REJECTED 2

#endif
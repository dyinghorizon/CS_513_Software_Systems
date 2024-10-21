#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD

#include <time.h>

struct Employee {
    int id; // 0, 1, 2, ....
    char name[25];
    char gender; // M -> Male, F -> Female, O -> Other
    int age;
    // Login Credentials
    char login[30]; // Format : name-id (name will be the first word in the structure member `name`)
    char password[30];
    // Role
    char role[10]; // "Employee" or "Manager"
};

#endif
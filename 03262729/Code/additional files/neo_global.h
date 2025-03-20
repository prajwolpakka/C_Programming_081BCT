#ifndef GLOBALS_H
#define GLOBALS_H

extern char logged_in_user_file[256];

typedef struct {
    unsigned int accountNumber;
    char username[100];
    char password[100];
    double balance;
} BankAccount;

#endif 
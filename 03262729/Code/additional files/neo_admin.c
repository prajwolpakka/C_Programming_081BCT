#include <stdio.h>
#include <stdlib.h>//for rand()
#include <time.h>
#include <stdbool.h>//to use bool to create function
#include <dirent.h>
#include <string.h>
#include "neo_time.h"
#include "neo_global.h"
#include "clear_screen.h" 
#include <windows.h>

char logged_in_user_file[256];

// Function to generate a random 8-digit account number
unsigned int generateAccountNumber() {
    return (rand() % 90000000) + 10000000; // Ensuring it's 8 digits
}

// Function to check if the account number already exists
bool accountNumberExists(int accNum) 
{
    DIR *folder = opendir("."); 
    struct dirent *file; 

    while ((file = readdir(folder)) != NULL)    
    {
        if (strstr(file->d_name, ".txt") != NULL)    
        {
            FILE *fptr; 
            fptr = fopen(file->d_name, "rb");   
            if (fptr == NULL)  
            {
                printf("This file '%s' couldn't be opened to check if same account number already exists\n", file->d_name);
                printf("\nChecking another file...\n");
                fclose(fptr);
                continue;
            }
            
            BankAccount d2;
            fread(&d2, sizeof(d2), 1, fptr);  

            if(d2.accountNumber == accNum)
            {
                fclose(fptr);
                closedir(folder);
                return true;
            }
            fclose(fptr);
        }
    }
   
    closedir(folder);
    return false;
}

int neo_admin()
 {

    srand(time(NULL));
    
    clear_screen();

    Beep(1000, 750);

    printf("\t\tWelcome To Money Laundry\t\t\n");
    printf("\nPlease enter the following details to create a new account:\n");

    BankAccount account;
    
    FILE *fp;
    
    // Ensure unique account number
    do {
        account.accountNumber = generateAccountNumber();
    } while (accountNumberExists(account.accountNumber));

    // Initialize balance to 0
    account.balance = 0;

    // Take user input
    printf("\nEnter the username you want to use\t\t\t:\t");
    scanf("%[^\n]s", account.username);

    while(1)
    {
    printf("\nEnter your password (atleast 8 characters long)\t\t:\t");
    scanf("%s", account.password);
    
    if(strlen(account.password) < 8)
    {
        printf("\nPassword should be atleast 8 characters long. Please try again\n");
        continue;
    }
    
    char confirm_password[100];
    printf("\nConfirm your password\t\t\t\t \t:\t");
    scanf("%s", confirm_password);
    
    if(strcmp(account.password, confirm_password) != 0)
    {
        printf("\nPasswords do not match. Please try again\n");
        continue;
    }
    break;
    }

    // Store account details
    char final_accountNumber[100];
    sprintf(final_accountNumber, "%u.txt", account.accountNumber);

    fp = fopen(final_accountNumber, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }
    
    fwrite(&account, sizeof(account), 1, fp);
    fclose(fp);

    printf("\nAccount Created Successfully!\n");
    
    strcpy(logged_in_user_file, final_accountNumber);
    FILE *fptr;
    fptr = fopen("BLUE.txt","a");
    fprintf(fptr,"\nAccount Number\t    : \t%u\n",account.accountNumber);
    fprintf(fptr,"Username\t\t\t: \t%s\n",strupr(account.username));
    fprintf(fptr,"Password\t\t\t: \t%s\n",account.password);
    fprintf(fptr,"Created at\t\t\t:\t");
    char* time = current_time();
    fprintf(fptr,"%s",time);
    fprintf(fptr,"\n\n");

    fclose(fptr);

    return 0;
}
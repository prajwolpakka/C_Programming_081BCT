#include <stdio.h>
#include <dirent.h>
#include <stdlib.h> 
#include <string.h> 
#include <conio.h>
#include <windows.h>
#include "neo_time.h" 
#include "neo_global.h" 
#include "clear_screen.h" 

BankAccount initial_user, transfering_user;

void account_details(BankAccount *);
void adding_balance(BankAccount *);
void withdraw_balance(BankAccount *);
void transfer_balance(BankAccount *, BankAccount *);
int  delete_account(BankAccount *);
void neo_user();


void neo_user()
{
    FILE *fptr;
    int choice;

    fptr = fopen(logged_in_user_file, "rb"); 
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }
    fread(&initial_user, sizeof(initial_user), 1, fptr);
    fclose(fptr);

    clear_screen(); 

    while (1) // Infinite loop
    {
        Beep(1000, 650); 

        printf("\n\t \t Welcome %s \n", strupr(initial_user.username));

        //while(getchar() != '\n'); // Clear the input buffer

        printf("\nPress 1 to view your account details          \n");
        printf("Press 2 to deposit amount to your account     \n");
        printf("Press 3 to withdraw amount from your account  \n");
        printf("Press 4 to transfer sum to another account    \n");
        printf("Press 5 to delete account                     \n");
        printf("Press 6 to exit\t\t\t\t      \n");
        printf("\nEnter your choice\t\t\t      :\t");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Please enter a number.\n");
            while (getchar() != '\n')
            {
                // its an empty loop to remove the characters from the buffer
            }
             // Clear the input buffer
            continue; // Continue to the next iteration of the loop
        }

        switch (choice)
        {
        case 1:
            account_details(&initial_user);
            break;
        case 2:
            adding_balance(&initial_user);
            break;
        case 3:
            withdraw_balance(&initial_user);
            break;
        case 4:
            transfer_balance(&initial_user, &transfering_user);
            break;
        case 5: 
            int re = delete_account(&initial_user);
              
            if(re == 1)
            {
                return;
            }

            break;
        case 6:
            return; // Exit the function, which breaks the loop
        default:
            printf("Please enter a valid choice\n");
            break;
        }
    }
}

void account_details(BankAccount *p)
{
    clear_screen(); 

    Beep(1000, 600);

    printf("\nYour account details are:\n");
    printf("\nAccount Number\t:\t %u\n", p->accountNumber);
    printf("Username\t:\t %s\n", p->username);
    printf("Password\t:\t %s\n", p->password);
    printf("Balance\t\t:\t %.2lf\n", p->balance);
    printf("\n\n");
    getch();
    clear_screen();
}

void adding_balance(BankAccount *p)
{
    clear_screen(); 

    FILE *fptr, *fptrTemp;
    double deposit_amt;

    while(1)
    {
        Beep(1000, 600);

        printf("\nEnter the amount you want to deposit    : ");
        if (scanf("%lf", &deposit_amt) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        break;
    }

    p->balance = p->balance + deposit_amt;

    printf("\nBalance is updated successfully\n");
    printf("\nYour new balance is\t\t\t: %.2lf\n", p->balance);

    fptr = fopen(logged_in_user_file, "wb"); 
    if (fptr == NULL)
    {
        perror("Error opening file"); // Error message uses errno to get error code and prints the error message
        return;
    }
    fptrTemp = fopen("temp.txt", "wb");
    if (fptrTemp == NULL)
    {
        perror("Error opening temp file"); 
        fclose(fptr);
        return;  /*exits the function early due to error*/
    }

    fwrite(&*p, sizeof(*p), 1, fptrTemp);

    fclose(fptr);
    fclose(fptrTemp);

    remove(logged_in_user_file);
    rename("temp.txt", logged_in_user_file); 

    getch();

    Beep(1000, 750);
    
    clear_screen();
    
}

void withdraw_balance(BankAccount *p)
{
    clear_screen(); 

    FILE *fptr, *fptrTemp;
    double withdraw_amt;

    while (1) // Loop to handle invalid input
    {
        Beep(1000, 600);

        printf("Enter the amount you want to withdraw   : ");
        if (scanf("%lf", &withdraw_amt) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        if (withdraw_amt > p->balance)
        {
            printf("Insufficient balance. Please enter a valid amount.\n");
            continue;
        }
        break;
    }

    p->balance = p->balance - withdraw_amt;

    fptr = fopen(logged_in_user_file, "wb"); 
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }
    fptrTemp = fopen("temp.txt", "wb");
    if (fptrTemp == NULL)
    {
        perror("Error opening temp file");
        fclose(fptr);
        return;
    }

    fwrite(&*p, sizeof(*p), 1, fptrTemp);

    printf("\nAmount withdrawn successfully from account.\n\n");
    printf("Your new balance is\t\t\t: %.2lf\n", p->balance);

    fclose(fptr);
    fclose(fptrTemp);

    remove(logged_in_user_file); 
    rename("temp.txt", logged_in_user_file); 

    getch();
    clear_screen();
    
}

void transfer_balance(BankAccount *p, BankAccount *pt)
{
    clear_screen(); 

    FILE *fptr, *fptrTransfer, *fptrTemp, *fptrTemp2;
    double transfer_amt;
    char transfer_acc_no[20];

    while (1) // Loop to handle invalid input
    {
        Beep(1000, 600);

        printf("Enter the amount you want to transfer\t\t\t  :\t");
        if (scanf("%lf", &transfer_amt) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        if (transfer_amt > p->balance)
        {
            printf("Insufficient balance. Please enter a valid amount.\n");
            continue;
        }
        break;
    }

    printf("Enter the account number you want to transfer the sum to  :\t");
    scanf("%s", &transfer_acc_no);

    char transfer_file_name[256];
    strcat(transfer_acc_no, ".txt"); 
    strcpy(transfer_file_name, transfer_acc_no);

    DIR *folder = opendir("."); 
    struct dirent *file; 

    while ((file = readdir(folder)) != NULL)    
    {
        if (strstr(file->d_name, ".txt") != NULL)    
        {
            if(strcmp(file->d_name, transfer_file_name) == 0)
            {
                fptrTransfer = fopen(transfer_file_name, "rb"); 
                if (fptrTransfer == NULL)
                {
                    perror("Error opening transfer file");
                    fclose(fptr);
                    closedir(folder);
                    return;
                }
                fread(&*pt, sizeof(*pt), 1, fptrTransfer);
                fclose(fptrTransfer);

                p->balance = p->balance - transfer_amt;
                pt->balance = pt->balance + transfer_amt;

                printf("\nAmount transferred successfully.\n\n");

                printf("Your new balance is\t\t\t\t\t  :\t%.2lf\n", p->balance);

                fptr = fopen(logged_in_user_file, "wb"); 
                if (fptr == NULL)
                {
                    perror("Error opening file");
                    closedir(folder);
                    return;
                }

                fptrTransfer = fopen(transfer_file_name, "wb"); 
                if (fptrTransfer == NULL)
                {
                    perror("Error opening transfer file");
                    closedir(folder);
                    fclose(fptr);
                    return;
                }

                fptrTemp = fopen("temp.txt", "wb");
                if (fptrTemp == NULL)
                {
                    perror("Error opening temp file");
                    fclose(fptr);
                    fclose(fptrTransfer);
                    closedir(folder);
                    return;
                }
                fptrTemp2 = fopen("temp2.txt", "wb");
                if (fptrTemp2 == NULL)
                {
                    perror("Error opening temp2 file");
                    fclose(fptr);
                    fclose(fptrTransfer);
                    fclose(fptrTemp);
                    closedir(folder);
                    return;
                }

                fwrite(&*p, sizeof(*p), 1, fptrTemp);
                fwrite(&*pt, sizeof(*pt), 1, fptrTemp2);

                fclose(fptr);
                fclose(fptrTransfer);
                fclose(fptrTemp);
                fclose(fptrTemp2);
                closedir(folder);

                remove(logged_in_user_file); 
                remove(transfer_file_name); 

                rename("temp.txt", logged_in_user_file); 
                rename("temp2.txt", transfer_file_name);
                
                getch();
                clear_screen();
                return;
            }     
        }
    }
    printf("\nTransfer Account number does not exist\n");
    closedir(folder);

    getch();
    clear_screen();
    return;
}

int delete_account(BankAccount *p)
{
    clear_screen(); 

    char confirm_password[100];

    Beep(1000, 600);
    
    printf("\nEnter your password to confirm account deletion\t\t:\t");
    scanf("%s", confirm_password);

    if(strcmp(confirm_password, p->password) == 0)
    {
        printf("\nPassword confirmed.\n");
        
        if (remove(logged_in_user_file) == 0)
        {
            FILE *fp;
            fp = fopen("Red.txt","a");
            fprintf(fp,"Account Number\t  :\t\t%u\n", p->accountNumber);
            fprintf(fp,"Username\t\t  : \t%s\n",strupr(p->username));
            fprintf(fp,"Password\t\t  : \t%s\n",p->password);
            fprintf(fp,"Balance\t\t\t  : \t%.2lf\n", p->balance);
            fprintf(fp,"Deleted at\t\t  :\t\t");
            char* time = current_time();
            fprintf(fp,"%s",time);
            fprintf(fp,"\n\n");
            fclose(fp);
            printf("\nDeleting account...\n");
            printf("\nAccount deleted successfully.\n");
            getch();
            return 1;
        }
        else
        {
            printf("Error deleting account.\n");
        }
        return 2;
    }
    else
    {
        printf("Passwords do not match, Account deletion cancelled\n");

        getch();
        clear_screen();
        return 2;
    }
}

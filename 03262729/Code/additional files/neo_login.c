#include <stdio.h>
#include <string.h>  // for using string functions
#include <dirent.h>  // for manipulating directories
#include <windows.h> // for using Beep()
#include "neo_global.h"  
#include "clear_screen.h"

struct login
{
    char pass[20];
    unsigned int acc_no;
} l[2];

char logged_in_user_file[256];

int neo_login()
{
    clear_screen();

    Beep(1000, 750);

    printf("\t\tLoging In\t\t\n");

    printf("\nEnter your Account Number\t:\t");
    scanf("%u", &l[1].acc_no);

    while(getchar() != '\n');
    {

    }
    ; 

    printf("Enter your Password\t\t:\t");
    scanf("%s", l[1].pass);

    DIR *folder = opendir("."); 
    struct dirent *file; 

    while ((file = readdir(folder)) != NULL)    
    {
        if (strstr(file->d_name, ".txt") != NULL)    
        {
            FILE *fp; 
            fp = fopen(file->d_name, "rb");   
            if (fp == NULL)  
            {
                fclose(fp);
                closedir(folder);
                return 0;
            }
            BankAccount d1;
            fread(&d1, sizeof(d1), 1, fp);  

            if ( (d1.accountNumber == l[1].acc_no) && (strcmp(d1.password, l[1].pass) == 0) )  
            {
                strcpy(logged_in_user_file, file->d_name);
                fclose(fp);
                closedir(folder);
                return 1;
            }
            fclose(fp);
        }
    }

    closedir(folder); 
    return 2; 
}
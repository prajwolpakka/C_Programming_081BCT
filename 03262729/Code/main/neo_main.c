#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "neo_login.c"
#include "neo_admin.c"
#include "neo_user.c"
#include "clear_screen.h"


int main()
{
    while(1)
    {
        clear_screen(); 

        Beep(1000, 750);
        
        printf("\t\tWelcome to Money Laundry\t\t\n");
        printf("\nPress 1 to log in.\t \n");
        printf("Press 2 to sign in.\t \n");
        printf("Press 3 to exit.\t\t \n");
        printf("\nEnter your choice\t:\t");
        int choice;
        scanf(" %d", &choice);
        while(getchar() != '\n'); // Clear the input buffer
        {
            // its an empty loop to remove the characters from the buffer
        }
        switch(choice)
        {
            case 1:  
                label:   
                int ans;
                ans = neo_login();
                {
                    if(ans == 0)
                    {
                        perror("Error opening file");
                        break;
                    }

                    if(ans == 1)
                    {
                        clear_screen();

                        printf("\nLogged in successfully\n\n");
            
                        neo_user();

                        clear_screen();
                        printf("Exited the system successfully\n");
                        Beep(1000, 750);
                        clear_screen();
                        break;
                    }
                    
                    if(ans == 2)
                    {
                        printf("\n\nEntered username and password doesnot match.");
                        printf("\n\nPress R to try again\t\t: ");
                        printf("\nPress C to create new account   : ");
                        printf("\nPress any other key to exit\t:\t");
                        char ch;
                        scanf(" %c",&ch);
                        if (ch == 'R' || ch == 'r')
                        {
                            goto label;
                        }
                        else if (ch == 'C' || ch == 'c')
                        { 
                            neo_admin();

                            neo_user();

                            clear_screen();
                            printf("Exited the system successfully\n");
                        
                            Beep(1000, 750);
                            clear_screen();
                            break;
                        }
                        else
                        {
                            clear_screen();
                            printf("Exited the system successfully\n");
                            Beep(1000, 750);
                            clear_screen();
                            break;
                        }
                    }

                }
            

            case 2:
                neo_admin();

                neo_user();

                clear_screen();
                printf("Exited the system successfully\n");
                Beep(1000, 750);
                clear_screen();
                
                break;

            case 3:
                clear_screen();
                printf("Exited the system successfully\n");

                Beep(1000, 750);
                
                clear_screen();

                break;

            default:
                printf("\nInvalid choice\n");

                while(getchar() != '\n'); // Clear the input buffer
                {
                    // its an empty loop to remove the characters from the buffer
                }
                getch();
                continue;
        }
        return 0;
    }
}
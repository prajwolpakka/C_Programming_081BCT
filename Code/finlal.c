//p[20]>players position in scoreboard function
//n>no. of players
//b[20]>condition for not printing you have unlocked your dies again and again
//z[20]>condition for not printing you have not unlocked your dies again and again 
//l>condition for not aking no. of players again and again
//c[20][20]>name of the players
//a[20]>position of the players in main function
//count>condition to stop the game if n-1 players reaches 100
//flow of code is : main>disc>scoreboard>main


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int disc();
void scoreboard(int p[20], int n);

int main()
{

   
    int a[20]; 
    static int b[20],z[20],l;
    int t, q, n, x;
    int i,count;
    char c[20][20];
    
x:
     if(l==0){
     printf("\033[1;32mGet ready for up's and down's\033[0m\n", i + 1);
     srand(time(NULL));
    printf("enter the no of player\n");
    scanf("%d", &n);
     }
    if (n < 2)
    {
        printf("enter the no of player above 2\n");
        goto x;
    }
    else{
        l=1;
    }
    system("cls");
    // assigning the variable for each palayer
    for(i=0;i<n;i++){
        a[i]=0;
    }
    for (i = 0; i < n; i++)
    {
        printf("\033[1;35m enter the name of player p[%d]  \033[0m\n", i+1);
        
        scanf("%s", c[i]);
    }
    do
    {
        count=0;
        for(i=0;i<n;i++){
            if(a[i]<100){
                count=count+1;

            }
            
        }
        
        if(count==1){
            printf("The game is finish here thankyou for playing\n");
            break;
        }
        for (i = 0; i < n; i++)
        {
            
            if(a[i]==100){
                
                 continue;

            }//if any player reach to 100 then we have to out them from game
           
            printf("\033[1;36m %s your turn  \033[0m\n", c[i]);
            
            system("pause");
            t = disc();          
            printf("you rolled %d \n", t);
            if(t==1&&b[i]==0){
                printf("you have unlocked your dies\n");
               z[i]=1;
               b[i]=1;
            }
            if(z[i]==0){
                printf("you haven't unlocked your dies\n");
                a[i]=0;
            }
             if(b[i]==1){
            a[i] = a[i] + t;          
            printf("%s your current position is %d\n", c[i], a[i]);
            if (a[i] > 100)
            {
                a[i] = a[i] - t;
            }
            if (a[i] == 45)
            {
                printf("\033[1;32m %s yeah lucky person you climbed up to 68 \033[0m\n", c[i]); // Green
                a[i] = 67;
            }
            if (a[i] == 76)
            {
                printf("\033[1;32m%s yeah lucky person you climbed up to 87 \033[0m\n", c[i]); // Green
                a[i] = 87;
            }
            if (a[i] == 12)
            {
                printf("\033[1;32m%s yeah lucky person you climbed up to 34 \033[0m\n", c[i]); // Green
                a[i] = 34;
            }
            if (a[i] == 56)
            {
                printf("\033[1;32m%s yeah lucky person you climbed up to 67 \033[0m\n", c[i]); // Green
                a[i] = 67;
            }
            if (a[i] == 85)
            {
                printf("\033[1;32m%s yeah lucky person you climbed up to 91\033[0m\n", c[i]); // Green
                a[i] = 91;
            }
            if (a[i] == 24)
            {
                printf("\033[1;32m%s you climbed up to 46 \033[0m\n", c[i]); // Green
                a[i] = 46;
            }
            if (a[i] == 99)
            {
                printf("\033[1;31m %s you fell down to 84 but still have chance  \033[0m\n", c[i]); // red
                a[i] = 84;
            }
            if (a[i] == 78)
            {
                printf("\033[1;31m%s you fell down to 64 but still have chance \033[0m\n", c[i]); // red
                a[i] = 64;
            }
            if (a[i] == 35)
            {
                printf("\033[1;31m%s you fell down to 18. frustrating!!! \033[0m\n", c[i]); // red
                a[i] = 18;
            }
            if (a[i] == 49)
            {
                printf("\033[1;31m%s you fell down to 28 , not a big fall  \033[0m\n", c[i]); // red
                a[i] = 28;
            }
            if (a[i] == 24)
            {
                printf("\033[1;31m%s you fell down to to 8. come on bruh rush up\033[0m\n", c[i]); // red
                a[i] = 8;
            }
            if (a[i] == 100)
            {
                printf("\033[1;35mplayer %d won the game \033[0m\n", i + 1); // red                          
            }  
        } 
        printf("\n");   
        }
        system("pause");
        system("cls");
        scoreboard(a, n);

    } while (a[0] !=100||a[1]!=100||a[2]!=100||a[3]!=100 );

    getchar();
    getch();
    return 0;
}

// calling the function for the scoreboard
void scoreboard(int p[20], int n)
{
    int i, j, pos, a, k,m,o,z;
    int l = 1, special = 0;
    // defing the value of pos from 1-100
    system("cls");

    for (i = 10; i >= 1; i--)
    {
        a = 1;

        for (k = 0; k < 38; k++)
        {
            printf("\033[1;36m--\033[0m");
        }
        printf("\n");

        for (j = 10; j >= 1; j--)
        {
            special = 0;
            if (i % 2 == 0)
            {
                pos = (i - 1) * 10 + j;
            }
            else
            {
                pos = (i - 1) * 10 + a;
                a++;
            }
            z=0;
            for(l=0;l<n;l++){
                if(p[l]==pos){
                    z++;
                }
            }
            //printing multiple player at the same position
            if(z>=2){
                printf("\033[1;30m|#%d#|\033[0m\t",z);
                special=1;
            }
            // printing scoreboard from 1-100          
            if(z==1){
            for (l = 0; l < n; l++)
            {
                
                if (pos == p[l])

                {   
                    o=33+l;//colour different for different player
                    
                    printf("\033[1;%dm|p%d|\033[0m\t",o, l + 1); // printing player position

                    special = 1;
                }
            }
        }
            if (special != 1)
            {
                // printing the different ladder(Green)
                if (pos == 45 || pos == 67)
                {
                    printf("\033[1;32m|L1|\033[0m\t");
                }
                else if (pos == 76 || pos == 87)
                {
                    printf("\033[1;32m|L2|\033[0m\t");
                }
                else if (pos == 12 || pos == 34)
                {
                    printf("\033[1;32m|L3|\033[0m\t");
                }
                else if (pos == 56 || pos == 68)
                {
                    printf("\033[1;32m|L4|\033[0m\t");
                }
                else if (pos == 85 || pos == 91)
                {
                    printf("\033[1;32m|L5|\033[0m\t");
                }
                else if (pos == 24 || pos == 46)
                {
                    printf("\033[1;32m|L6|\033[0m\t");
                }
                // printing the different snake(red)
                else if (pos == 99 || pos == 84)
                {
                    printf("\033[1;31m|S1|\033[0m\t");
                }
                else if (pos == 78 || pos == 64)
                {
                    printf("\033[1;31m|S2|\033[0m\t");
                }
                else if (pos == 35 || pos == 18)
                {
                    printf("\033[1;31m|S3|\033[0m\t");
                }
                else if (pos == 49 || pos == 28)
                {
                    printf("\033[1;31m|S4|\033[0m\t");
                }
                else if (pos == 24 || pos == 8)
                {
                    printf("\033[1;31m|S5|\033[0m\t");
                }
                else if (pos == 18 || pos == 4)
                {
                    printf("\033[1;31m|S6|\033[0m\t");
                }

                else
                {
                    printf("|%d|\t", pos);
                }
            }
        }
        printf("\n");
    }
    for (k = 0; k < 38; k++)
    {
        printf("\033[1;36m--\033[0m");
    }
    printf("\n");
    system("pause");
    system("cls");
}
int disc()
{
    return (rand() % 6) + 1; // Generates a number between 1 and 6
}
//Here the last function is disc which is used to generate the random number between 1-6

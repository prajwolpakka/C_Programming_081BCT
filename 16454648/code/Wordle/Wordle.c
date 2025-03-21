#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
struct stats
{
    int total;
    int winstreak;
    int maxstreak;
    int one;
    int two;
    int three;
    int four;
    int five;
    int six;
};
#include "Functions/startgame.h"
#include "Functions/howtoplay.h"
#include "Functions/stats.h"
#include "Functions/exitgame.h"
void green(char c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
    printf("  %c  ", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void green_thin(char c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
    printf("%c", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void yellow(char c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_RED);
    printf("  %c  ", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void white(char c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
    printf("  %c  ", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void printfgreen(char c[100])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | 0);
    printf("%s", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void printfred(char c[100])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | 0);
    printf("%s", c);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | 0);
}
void title()
{
    system("cls");
    printf("\n");
    printf("  ");
    green('W');
    printf("   ");
    yellow('O');
    printf("   ");
    white('R');
    printf("   ");
    green('D');
    printf("   ");
    yellow('L');
    printf("   ");
    white('E');
    printf("\n\n\n");
}
void menu1()
{
    title();
    printf("\t\t  ");
    printfgreen("Start Game");
    printf("\n\t\t  ");
    printf("How To Play");
    printf("\n\t\t  ");
    printf("Stats");
    printf("\n\t\t  ");
    printf("Exit Game");
    printf("\n\n\n\n   1->Previous       2->Next       Enter->Select");
}
void menu2()
{
    title();
    printf("\t\t  ");
    printf("Start Game");
    printf("\n\t\t  ");
    printfgreen("How To Play");
    printf("\n\t\t  ");
    printf("Stats");
    printf("\n\t\t  ");
    printf("Exit Game");
    printf("\n\n\n\n   1->Previous       2->Next       Enter->Select");
}
void menu3()
{
    title();
    printf("\t\t  ");
    printf("Start Game");
    printf("\n\t\t  ");
    printf("How To Play");
    printf("\n\t\t  ");
    printfgreen("Stats");
    printf("\n\t\t  ");
    printf("Exit Game");
    printf("\n\n\n\n   1->Previous       2->Next       Enter->Select");
}
void menu4()
{
    title();
    printf("\t\t  ");
    printf("Start Game");
    printf("\n\t\t  ");
    printf("How To Play");
    printf("\n\t\t  ");
    printf("Stats");
    printf("\n\t\t  ");
    printfgreen("Exit Game");
    printf("\n\n\n\n   1->Previous       2->Next       Enter->Select");
}
int main()
{
    int option = 1;
    char c;
top:
    c = '0';
    while (c != '\r')
    {
        switch (option)
        {
        case 1:
            menu1();
            break;
        case 2:
            menu2();
            break;
        case 3:
            menu3();
            break;
        case 4:
            menu4();
            break;
        }
        c = getch();
        if (c == '2' && option != 4)
        {
            option++;
        }
        else if (c == '1' && option != 1)
        {
            option--;
        }
    }
    switch (option)
    {
    case 1:
        startgame();
        goto top;
        break;
    case 2:
        howtoplay();
        goto top;
        break;
    case 3:
        stats();
        goto top;
        break;
    case 4:
        exitgame();
        break;
    }
    return 0;
}

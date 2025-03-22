#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10
#define MAX_O 5
#define MAX_MOVES 60

struct Point {
    int x, y;
};

void drawGame(struct Point player, struct Point objects[], int objCount, int score, int movesLeft) {
    for (int i = 0; i < HEIGHT + 5; i++) 
    {
        printf("\n");
    }

    for (int i = 0; i < HEIGHT + 2; i++) 
    {
        for (int j = 0; j < WIDTH + 2; j++) 
        {
            if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) 
            {
                printf("#");
            } 
            else 
            {
                int isObject = 0;
                for (int k = 0; k < objCount; k++) 
                {
                    if (objects[k].x == j - 1 && objects[k].y == i - 1) 
                    {
                        printf("O");
                        isObject = 1;
                        break;
                    }
                }

                if (!isObject) 
                {
                    if (player.x == j - 1 && player.y == i - 1) 
                    {
                        printf("P");
                    } 
                    else 
                    {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }

    printf("Score: %d  Moves Left: %d\n", score, movesLeft);
    printf("Use WASD to move, Q to quit\n");
    printf("Enter move: ");
}

void generateRandomObjects(struct Point objects[], int objCount, struct Point player) 
{
    for (int i = 0; i < objCount; i++) 
    {
        do 
        {
            objects[i].x = rand() % WIDTH;
            objects[i].y = rand() % HEIGHT;
        } 
        while (objects[i].x == player.x && objects[i].y == player.y);
    }
}

int main() 
{
    struct Point player = {WIDTH / 2, HEIGHT / 2};
    struct Point objects[MAX_O];
    int score = 0, movesLeft = MAX_MOVES;
    char input;

    srand(time(NULL));
    generateRandomObjects(objects, MAX_O, player);

    while (movesLeft > 0) {
        drawGame(player, objects, MAX_O, score, movesLeft);

        input = getch();
        movesLeft--;

        switch (input) 
        {
            case 'w': if (player.y > 0) player.y--;
                break;
            case 's': if (player.y < HEIGHT - 1) player.y++;
                break;
            case 'a': if (player.x > 0) player.x--; 
                break;
            case 'd': if (player.x < WIDTH - 1) player.x++; 
                break;
            case 'q': case 'Q':
                printf("\nGame Quit! Final Score: %d\n", score);
                return 0;
        }

        for (int i = 0; i < MAX_O; i++) 
        {
            if (player.x == objects[i].x && player.y == objects[i].y) 
            {
                score++;
                generateRandomObjects(&objects[i], 1, player);
            }
        }
    }

    printf("\nMoves exhausted! Final Score: %d\n", score);
    printf("Press Enter to exit...");
    while (getchar() != '\n');
    getchar();

    return 0;
}

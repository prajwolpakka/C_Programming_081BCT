void wordpicker(char word[5])
{
    srand(time(0));
    int n = rand() % 2309;
    FILE *fp;
    fp = fopen("TextFiles/wordleanswers.txt", "r");
    fseek(fp, n * 7, 0);
    fscanf(fp, "%s", word);
    fclose(fp);
}
int validword(char guess[5])
{
    guess[5] = '\0';
    FILE *fp;
    fp = fopen("TextFiles/totalwords.txt", "r");
    char word[5];
    int upr = 12946, lwr = 0, mid;
    fseek(fp, upr * 7, 0);
    fscanf(fp, "%s", word);
    strupr(word);
    if (strcmp(word, guess) == 0)
    {
        fclose(fp);
        return 0;
    }
    fseek(fp, lwr * 7, 0);
    fscanf(fp, "%s", word);
    strupr(word);
    if (strcmp(word, guess) == 0)
    {
        fclose(fp);
        return 0;
    }
    while ((upr - lwr) > 1)
    {
        mid = (upr + lwr) / 2;
        fseek(fp, mid * 7, 0);
        fscanf(fp, "%s", word);
        strupr(word);
        if (strcmp(word, guess) == 0)
        {
            fclose(fp);
            return 0;
        }
        else if (strcmp(word, guess) < 0)
            lwr = (upr + lwr) / 2;
        else
            upr = (upr + lwr) / 2;
    }
    fclose(fp);
    return 1;
}
void wordchecker(char word[5], char guess[5], int letter_status[5])
{
    int i, j;
    for (i = 0; i < 5; i++)
    {
        letter_status[i] = 0;
    }
    for (i = 0; i < 5; i++)
    {
        if (word[i] == guess[i])
        {
            letter_status[i] = 2;
            word[i] = '0';
        }
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (guess[i] == word[j] && letter_status[i] == 0)
            {
                letter_status[i] = 1;
                word[j] = '0';
            }
        }
    }
}
void statswin(int guess_count)
{
    FILE *fp;
    struct stats s;
    fp = fopen("TextFiles/stats.txt", "r");
    fread(&s, sizeof(s), 1, fp);
    s.total++;
    s.winstreak++;
    if (s.winstreak > s.maxstreak)
        s.maxstreak = s.winstreak;
    switch (guess_count)
    {
    case 1:
        s.one++;
        break;
    case 2:
        s.two++;
        break;
    case 3:
        s.three++;
        break;
    case 4:
        s.four++;
        break;
    case 5:
        s.five++;
        break;
    case 6:
        s.six++;
        break;
    }
    fclose(fp);
    fp = fopen("TextFiles/stats.txt", "w");
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
}
void statslose()
{
    FILE *fp;
    struct stats s;
    fp = fopen("TextFiles/stats.txt", "r");
    fread(&s, sizeof(s), 1, fp);
    s.total++;
    s.winstreak = 0;
    fclose(fp);
    fp = fopen("TextFiles/stats.txt", "w");
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
}
void startgame()
{
    title();
    char word[5], guess[8][5], temp_word[5];
    int guess_count, letter, c, valid = 0, i, letter_status[5];
    wordpicker(word);
    strupr(word);
    printf("\t\t Guess The Word!\n\n\t  ");
    for (guess_count = 1; guess_count <= 7; guess_count++)
    {
        letter = 0;
        while (valid == 0)
        {
            c = getch();
            if (c == '\r')
            {
                if (letter < 5)
                {
                    for (i = 1; i < 5 - letter; i++)
                    {
                        printf("      ");
                    }
                    printfred("        Too short");
                    sleep(1);
                    printf("\b\b\b\b\b\b\b\b\b         \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
                    for (i = 1; i < 5 - letter; i++)
                    {
                        printf("\b\b\b\b\b\b");
                    }
                }
                else if (validword(guess[guess_count]) == 1)
                {
                    printfred("   Word not found");
                    sleep(1);
                    printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b              \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
                }
                else
                {
                    printf("\b\b\b\b \b\b\b\b\b\b\b \b\b\b\b\b\b\b \b\b\b\b\b\b\b \b\b\b\b\b\b\b \b\b\b");
                    break;
                }
            }
            else if (letter < 5 || c == '\b')
            {
                if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                {
                    if (c >= 97 && c <= 122)
                    {
                        c = c - 32;
                    }
                    printf("  %c   ", c);
                    guess[guess_count][letter] = c;
                    letter++;
                }
                else if (c == '\b' && letter > 0)
                {
                    printf("\b\b\b\b \b\b\b");
                    letter--;
                }
            }
        }
        strcpy(temp_word, word);
        wordchecker(temp_word, guess[guess_count], letter_status);
        for (i = 0; i < 5; i++)
        {
            if (letter_status[i] == 2)
            {
                green(guess[guess_count][i]);
            }
            else if (letter_status[i] == 1)
            {
                yellow(guess[guess_count][i]);
            }
            else
            {
                white(guess[guess_count][i]);
            }
            printf(" ");
        }
        printf("\n\n\t  ");
        if (guess_count <= 6)
        {
            if (strcmp(word, guess[guess_count]) == 0)
                break;
            else if (strcmp(word, guess[guess_count]) != 0 && guess_count == 6)
            {
                guess_count++;
                break;
            }
        }
    }
    for (i = 1; i <= 2 * (6 - guess_count); i++)
    {
        printf("\n");
    }
    printf("\n\t\t  ");
    switch (guess_count)
    {
    case 1:
        printfgreen("   Genius!\n");
        break;
    case 2:
        printfgreen(" Magnificent!\n");
        break;
    case 3:
        printfgreen(" Impressive!\n");
        break;
    case 4:
        printfgreen("  Splendid!\n");
        break;
    case 5:
        printfgreen("    Great!\n");
        break;
    case 6:
        printfgreen("    Phew!\n");
        break;
    default:
        printfred("  You Lost!\n");
        break;
    }
    if (guess_count <= 6)
    {
        printf("\t      You guessed the word.");
        statswin(guess_count);
    }
    else
    {
        printf("\t         The answer was:\n\t\t");
        printf("      %s", word);
        statslose();
    }
    printf("\n\n\n      Press any key to return to main menu.");
    getch();
}

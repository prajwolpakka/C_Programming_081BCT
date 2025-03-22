void reset()
{
    struct stats s;
    FILE *fp;
    fp = fopen("TextFiles/stats.txt", "w+");
    s.total = 0;
    s.winstreak = 0;
    s.maxstreak = 0;
    s.one = 0;
    s.two = 0;
    s.three = 0;
    s.four = 0;
    s.five = 0;
    s.six = 0;
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
}
void p_display(int per)
{
    int i;
    int p;
    char c = ' ';
    p = (per * 38) / 100;
    for (i = 0; i < 41; i++)
    {
        printf("\b");
    }
    for (i = 1; i <= p; i++)
    {
        green_thin(c);
    }
}
void stats()
{
stat:
    title();
    struct stats s;
    FILE *fp;
    fp = fopen("TextFiles/stats.txt", "r+");
    fread(&s, sizeof(s), 1, fp);
    fclose(fp);
    int wins, best_try;
    float win_percent, one_p, two_p, three_p, four_p, five_p, six_p;
    char c = 'p';
    wins = s.one + s.two + s.three + s.four + s.five + s.six;
    if (s.one > 0)
        best_try = 1;
    else if (s.two > 0)
        best_try = 2;
    else if (s.three > 0)
        best_try = 3;
    else if (s.four > 0)
        best_try = 4;
    else if (s.five > 0)
        best_try = 5;
    else if (s.six > 0)
        best_try = 6;
    else
        best_try = 0;
    if (wins != 0)
    {
        win_percent = ((float)wins / (float)s.total) * 100;
        one_p = ((float)s.one / (float)wins) * 100;
        two_p = ((float)s.two / (float)wins) * 100;
        three_p = ((float)s.three / (float)wins) * 100;
        four_p = ((float)s.four / (float)wins) * 100;
        five_p = ((float)s.five / (float)wins) * 100;
        six_p = ((float)s.six / (float)wins) * 100;
    }
    else
    {
        win_percent = 0;
        one_p = 0;
        two_p = 0;
        three_p = 0;
        four_p = 0;
        five_p = 0;
        six_p = 0;
    }
    printf("\t\t    Statistics\n\n");
    printf("   GAMES PLAYED\t    GAMES WON\t    %% OF WIN\n  ");
    printf("      %d\t\t%d\t       %.0f%%", s.total, wins, win_percent);
    printf("\n\n     BEST TRY  \t  CURRENT STREAK   MAX STREAK\n  ");
    printf("      #%d\t\t%d\t\t%d\n\n\n", best_try, s.winstreak, s.maxstreak);
    printf("\t\tGUESS DISTRIBUTION\n");
    printf("\n  #1\t\t\t\t\t      %d", s.one);
    p_display(one_p);
    printf("\n\n  #2\t\t\t\t\t      %d", s.two);
    p_display(two_p);
    printf("\n\n  #3\t\t\t\t\t      %d", s.three);
    p_display(three_p);
    printf("\n\n  #4\t\t\t\t\t      %d", s.four);
    p_display(four_p);
    printf("\n\n  #5\t\t\t\t\t      %d", s.five);
    p_display(five_p);
    printf("\n\n  #6\t\t\t\t\t      %d", s.six);
    p_display(six_p);
    printf("\n\n  R->Reset stats\t\t0->Return to menu\n");
    while (c != '0')
    {
        c = getch();
        if (c == 'r' || c == 'R')
        {
            printf("  Are you sure? [Enter] to proceed.");
            c = getch();
            if (c == '\r')
                reset();
            goto stat;
        }
        else if (c == '0')
        {
            break;
        }
    }
}
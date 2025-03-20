#ifndef CLEAR_SCREEN_H
#define CLEAR_SCREEN_H

#include <stdio.h>

void clear_screen()
{
    printf("\x1b[2J");
    printf("\x1b[1;1f");
}

#endif
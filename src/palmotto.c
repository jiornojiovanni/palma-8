#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include "interpreter.h"

int main(int argc, char const *argv[])
{

    FILE *fp;

    VM state = initState();

    if(argc > 1) 
    {
        fp = fopen(argv[1], "rb");
    } 
    else
    {
        printf("Enter the name of the rom as an argument.");
        return -1;
    }

    if (fp == NULL)
    {
        printf("Error during file reading.");
        return -1;
    }
    fread(state.RAM + OFFSET, sizeof(state.RAM), 1, fp);
    fclose(fp);
    
    initscr();
    start_color();
    curs_set(FALSE);
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    attron(COLOR_PAIR(1));

    while (1)
    {
        clear();
        cicle(&state);
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (state.video[i][j] == 1)
                {
                    mvaddch(i, j, 1);
                }
            }
        }
        refresh();
        usleep(2000);
    }
}
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>
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

    while (1)
    {
        cicle(&state);
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                if (state.video[i][j] == '*')
                {
                    mvaddch(i, j, '*');
                }
            }
        }
        refresh();
    }
}
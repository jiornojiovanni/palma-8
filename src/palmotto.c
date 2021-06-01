#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "interpreter.h"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define KBSIZE 16

int main(int argc, char const *argv[])
{

    FILE *fp;
    VM state = initState();
    char KEYBOARD[KBSIZE] = {'1', '2', '3', '4', 'q', 'w', 'e', 'r', 'a', 's', 'd', 'f', 'z', 'x', 'c', 'v'};

    if (argc > 1)
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

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *gWindow = SDL_CreateWindow("PALMOTTO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    SDL_Renderer *gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(gRenderer, COLUMNS, ROWS);

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        if (state.KBinterrupt)
        {
            SDL_WaitEvent(&e);

            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            for (int i = 0; i < KBSIZE; i++)
            {
                if (e.key.keysym.sym == KEYBOARD[i])
                {
                    state.KB[i] = 0xFF;
                    state.V[state.KBrequest] = e.key.keysym.sym;
                    break;
                }
            }

            if (e.type == SDL_KEYUP)
            {
                for (int i = 0; i < KBSIZE; i++)
                {
                    if (e.key.keysym.sym == KEYBOARD[i])
                    {
                        state.KB[i] = 0x00;
                    }
                }
            }
        }
        else
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }

                for (int i = 0; i < KBSIZE; i++)
                {
                    if (e.key.keysym.sym == KEYBOARD[i])
                    {
                        state.KB[i] = 0xFF;
                    }
                }

                if (e.type == SDL_KEYUP)
                {
                    for (int i = 0; i < KBSIZE; i++)
                    {
                        if (e.key.keysym.sym == KEYBOARD[i])
                        {
                            state.KB[i] = 0x00;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        cicle(&state);
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (state.video[i][j] == 1)
                {
                    SDL_RenderDrawPoint(gRenderer, j, i);
                }
            }
        }
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
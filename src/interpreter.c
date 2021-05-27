#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

VM initState()
{
    VM initialState = {
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        },
        0,
        0,
        0,
        {0},
        {0},
    };
    return initialState;
}

void cicle(VM *state)
{
    unsigned char instruction;
    unsigned short opcode;
    unsigned char height;
    instruction = state->RAM[state->PC + OFFSET];
    opcode = (state->RAM[state->PC + OFFSET] << 8) | state->RAM[state->PC + OFFSET + 1];

    switch (instruction >> 4)
    {
    case 0x0A: //ANNN Sets I to the address NNN.
        state->I = opcode & 0x0FFF;
        state->PC += 2;
        break;
    case 0x06: //6XNN Sets VX to NN.
        state->V[instruction & 0x0F] = opcode & 0x00FF;
        state->PC += 2;
        break;
    case 0x0D: //DXYN Draws a sprite at coordinate (VX, VY) with height N
        height = opcode & 0x000F;
        int collisionDetected = 0;
        for (int i = 0; i < height; i++)
        {
            char row = state->RAM[state->I + i] >> 4;

            //We check each row bit to bit, from left to right. (for example a row with value 1111 we shift initially 3 bit so we get 1 then 2 bit then 1. )
            int bit = 3, j = 0;
            do
            {
                if ((row >> bit) & 0x01)
                {
                    int vX = (opcode & 0x0F00) >> 8;
                    int vY = (opcode & 0x00F0) >> 4;
                    if (state->video[state->V[vX] + i][state->V[vY] + j] == '*')
                    {
                        state->video[state->V[vX] + i][state->V[vY] + j] = 0;
                        collisionDetected = 1;
                    }
                    else
                    {
                    state->video[state->V[vX] + i][state->V[vY] + j] = '*';
                    }
                }
                j++;
                bit--;
            } while (bit > -1);
        }

        if (collisionDetected)
        {
            state->V[0xF] = 1;
        }
        else
        {
            state->V[0xF] = 0;
        }

        state->PC += 2;
        break;
    case 0x01: //1NNN Jumps to address NNN.
        state->PC = (opcode & 0x0FFF) - OFFSET;
        break;
    default:
        printf("Invalid instruction: %x", state->RAM[state->PC] >> 4);
        exit(-1);
        break;
    }
}
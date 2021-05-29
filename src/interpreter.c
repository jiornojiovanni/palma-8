#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void halt(unsigned char instruction)
{
    printf("Invalid instruction: %x", instruction);
    exit(-1);
}

void cicle(VM *state)
{
    unsigned char instruction, nextInstruction, height;
    unsigned short opcode;
    instruction = state->RAM[state->PC + OFFSET];
    nextInstruction = state->RAM[state->PC + OFFSET + 1];
    opcode = (instruction << 8) | nextInstruction;

    switch (instruction >> 4)
    {
    case 0x00:
        switch (nextInstruction)
        {
        case 0xE0: //00E0 Clears the screen.
            memset(state->video, 0, 64 * 32);
            state->PC += 2;
            break;

        default:
            halt(instruction);
            break;
        }
        break;

    case 0x01: //1NNN Jumps to address NNN.
        state->PC = (opcode & 0x0FFF) - OFFSET;
        break;

    case 0x03: //3XNN Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block).
        state->PC += state->V[instruction & 0x0F] == nextInstruction ? 4 : 2;
        break;

    case 0x04: //4XNN Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block).
        state->PC += state->V[instruction & 0x0F] != nextInstruction ? 4 : 2;
        break;

    case 0x05: //5XY0 Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block).
        state->PC += state->V[instruction & 0x0F] == state->V[nextInstruction >> 4] ? 4 : 2;
        break;

    case 0x06: //6XNN Sets VX to NN.
        state->V[instruction & 0x0F] = opcode & 0x00FF;
        state->PC += 2;
        break;

    case 0x07: //7XNN Add NN to Vx (without carry flag change).
        state->V[instruction & 0x0F] += opcode & 0x00FF;
        state->PC += 2;
        break;

    case 0x08:
        switch (nextInstruction & 0x0F)
        {
        case 0x00: //8XY0 Sets VX to the value of VY.
            state->V[instruction & 0x0F] = state->V[nextInstruction >> 4];
            state->PC += 2;
            break;

        case 0x01: //8XY1 Sets VX to VX or VY.
            state->V[instruction & 0x0F] = state->V[instruction & 0x0F] | state->V[nextInstruction >> 4];
            state->PC += 2;
            break;

        case 0x02: //8XY2 Sets VX to VX and VY.
            state->V[instruction & 0x0F] = state->V[instruction & 0x0F] & state->V[nextInstruction >> 4];
            state->PC += 2;
            break;

        case 0x03: //8XY3 Sets VX to VX xor VY.
            state->V[instruction & 0x0F] = state->V[instruction & 0x0F] ^ state->V[nextInstruction >> 4];
            state->PC += 2;
            break;

        case 0x04: //8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
            state->V[instruction & 0x0F] += state->V[nextInstruction >> 4];
            state->V[0xF] = state->V[instruction & 0x0F] + state->V[nextInstruction >> 4] > 255 ? 1 : 0;
            state->PC += 2;
            break;

        case 0x05: //8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
            state->V[0xF] = state->V[instruction & 0x0F] > state->V[nextInstruction >> 4] ? 1 : 0;
            state->V[instruction & 0x0F] -= state->V[nextInstruction >> 4];
            state->PC += 2;
            break;

        case 0x06: //8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
            state->V[0xF] = state->V[instruction & 0x0F] & 1;
            state->V[instruction & 0x0F] = state->V[instruction & 0x0F] >> 1;
            state->PC += 2;
            break;

        case 0x07: //8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
            state->V[0xF] = state->V[nextInstruction >> 4] > state->V[instruction & 0x0F] ? 1 : 0;
            state->V[nextInstruction >> 4] -= state->V[instruction & 0x0F];
            state->PC += 2;
            break;

        case 0x0E: //8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
            state->V[0xF] = state->V[instruction & 0x0F] >> 3;
            state->V[instruction & 0x0F] = state->V[instruction & 0x0F] << 1;
            state->PC += 2;
            break;

        default:
            halt(instruction);
            break;
        }
        break;

    case 0x09: //9XY0 Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block).
        state->PC += state->V[instruction & 0x0F] != state->V[nextInstruction >> 4] ? 4 : 2;
        break;

    case 0x0A: //ANNN Sets I to the address NNN.
        state->I = opcode & 0x0FFF;
        state->PC += 2;
        break;

    case 0x0B: //BNNN Jumps to the address NNN plus V0.
        state->PC = ((opcode & 0x0FFF) + state->V[0x00]) - OFFSET;
        break;

    case 0x0C: //CXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        state->V[instruction & 0x0F] = (rand() % 256) & nextInstruction;
        state->PC += 2;
        break;

    case 0x0D: //DXYN Draws a sprite at coordinate (VX, VY) with height N.
        height = opcode & 0x000F;
        int collisionDetected = 0;
        for (int i = 0; i < height; i++)
        {
            char row = state->RAM[state->I + i] >> 4;

            //We check each row bit to bit, from left to right.
            //(for example a row with value 1111 we shift initially 3 bit so we get 1 then 2 bit then 1. ).
            int bit = 3, j = 0;
            do
            {
                if ((row >> bit) & 0x01)
                {
                    int vX = (opcode & 0x0F00) >> 8;
                    int vY = (opcode & 0x00F0) >> 4;
                    if (state->video[state->V[vX] + i][state->V[vY] + j] == 1)
                    {
                        state->video[state->V[vX] + i][state->V[vY] + j] = 0;
                        collisionDetected = 1;
                    }
                    else
                    {
                        state->video[state->V[vX] + i][state->V[vY] + j] = 1;
                    }
                }
                j++;
                bit--;
            } while (bit > -1);
        }

        state->V[0xF] = collisionDetected ? 1 : 0;

        state->PC += 2;
        break;

    default:
        halt(instruction);
        break;
    }
}

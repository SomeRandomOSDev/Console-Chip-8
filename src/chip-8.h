#pragma once

#include <time.h>

#define KB 1024

struct Chip8
{
    uint8_t memory[4 * KB];
    uint8_t V[16];
    uint16_t I;
    uint8_t DT, ST; // delay timer, sound timer

    uint16_t PC;    // program counter
    uint8_t SP;     // stack pointer

    uint16_t stack[16];

    bool screen[64 * 32];

    uint16_t op;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;
    uint8_t X;
    uint8_t Y;
    uint8_t nibble_1000;
    uint8_t nibble_0100;
    uint8_t nibble_0010;
    uint8_t nibble_0001;

    char log[16];
};

const uint8_t C8_font[5 * 16] =     // 16x 8x5 sprites
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};

int keymap[16] =
{
    VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, 
    VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, 
    VK_NUMPAD8, VK_NUMPAD9, 0x41,       0x42, 
    0x43,       0x44,       0x45,       0x46
};

#include "chip-8_instructions.h"

void C8_init(struct Chip8* c8)
{
    c8->PC = 0x200;
    c8->I = 0;
    c8->DT = c8->ST = 0;
    c8->SP = 0;

    memset(&c8->V[0], 0, 16);
    memcpy(&c8->memory[0], &C8_font[0], 5 * 16);
    memset(&c8->screen[0], false, 64 * 32);

    srand(time(0));
}

void C8_loadROM(struct Chip8* c8, char* file)
{
    FILE* f = fopen(file, "rb");

    fseek(f, 0, SEEK_END);
	uint16_t fileLength = ftell(f);
	fseek(f, 0, SEEK_SET);

    fileLength = min(fileLength, (4 * KB) - 0x200);

    fread(&c8->memory[0x200], fileLength, 1, f);
}

void C8_cycle(struct Chip8* c8)
{
    //fetch
    uint8_t op_hi = c8->memory[c8->PC];
    uint8_t op_lo = c8->memory[c8->PC + 1];
    c8->op = ((op_hi << 8) | op_lo);
    c8->PC += 2;

    //decode
    c8->NNN = (c8->op & 0x0fff);
    c8->NN = (c8->op & 0xff);
    c8->N = (c8->op & 0x0f);
    c8->X = (op_hi & 0x0f);
    c8->Y = (op_lo >> 4);

    c8->nibble_1000 = (op_hi >> 4);
    c8->nibble_0100 = c8->X;
    c8->nibble_0010 = c8->Y;
    c8->nibble_0001 = (op_lo & 0x0f);

    char buf[4];
    strcpy(c8->log, "0x");
    itoa(c8->PC - 2, buf, 16);
    strcat(c8->log, buf);
    strcat(c8->log, " : 0x");
    itoa(c8->op, buf, 16);
    strcat(c8->log, &buf[0]);
    strcat(c8->log, " : ");

    //execute
    void (*op_handler)(struct Chip8* c8) = instructions_first_nibble[c8->nibble_1000];
    if(op_handler == NULL)
        UNHANDLED_OPCODE
    else
        (*op_handler)(c8);

    strcat(c8->log, "\n");

    if(c8->DT > 0)
        c8->DT--;
    if(c8->ST > 0)
        c8->ST--;
}

void C8_renderToScreenBuffer(struct Chip8* c8, char darkChar, char lightChar)
{
    for(uint8_t i = 0; i < 32; i++)
    {
        for(uint8_t j = 0; j < 64; j++)
        {
            char c = c8->screen[i * 64 + j] ? lightChar : darkChar;
            SetScreenBufferCharacter(j, i, c);
        }
    }
}
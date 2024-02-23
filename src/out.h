#pragma once

#include <windows.h>

#define SCREEN_WIDTH    (64 * 2)   // The aspect ratio of the characters is ~2x1
#define SCREEN_HEIGHT   32

HANDLE consoleHandle;
HANDLE consoleBufferHandle;
uint8_t screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
COORD origin = {0, 0};
DWORD bytesWritten;

void InitConsole()
{
    consoleBufferHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(consoleBufferHandle);
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

char GetScreenBufferCharacter(uint8_t x, uint8_t y)
{
    return screenBuffer[y * SCREEN_WIDTH + x];
}

void SetScreenBufferCharacter(uint8_t x, uint8_t y, char c)
{
    screenBuffer[y * SCREEN_WIDTH + x] = c;
}

void ClearScreenBuffer(char c)
{
    for(uint8_t i = 0; i < SCREEN_HEIGHT; i++)
    {
        for(uint8_t j = 0; j < SCREEN_WIDTH; j++)
        {
            SetScreenBufferCharacter(j, i, c);
        }
    }
}

void DisplayScreenBuffer()
{
    WriteConsoleOutputCharacter(consoleBufferHandle, &screenBuffer[0], SCREEN_WIDTH * SCREEN_HEIGHT, origin, &bytesWritten);
}
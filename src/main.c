#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "out.h"
#include "chip-8.h"

#define HANDLE_KEY(var, vKey)   { if(GetAsyncKeyState(vKey) >> 7) var++; else var = 0; }

enum RenderingMode
{
    Screen,
    Instructions
};

enum RenderingMode mode = Screen;

int main(int argc, char** argv)
{
    if(argc != 2)
        return 1;

    if(mode == Screen)
        InitConsole();

    struct Chip8 emu;
    C8_init(&emu);
    C8_loadROM(&emu, argv[1]);

    // uint64_t sDown = 0, spaceDown = 0;

    bool running = true;

    while(true)
    {
        // HANDLE_KEY(sDown, 0x53);
        // HANDLE_KEY(spaceDown, VK_SPACE);

        // running ^= (spaceDown == 1);

        // if(sDown == 1 || running)
        {
            C8_cycle(&emu);

            if(mode == Instructions)
                printf("%s", emu.log);
        }
        
        if(mode == Screen)
        {
            ClearScreenBuffer(' ');

            C8_renderToScreenBuffer(&emu, ' ', 219);

            DisplayScreenBuffer();
        }

        Sleep(1000 / 60);
    }
}
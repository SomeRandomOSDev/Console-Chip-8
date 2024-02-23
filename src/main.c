#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "out.h"
#include "chip-8.h"

int main(int argc, char** argv)
{
    if(argc != 2)
        return 1;

    // InitConsole();

    struct Chip8 emu;
    C8_init(&emu);
    C8_loadROM(&emu, argv[1]);

    uint32_t sDown = 0;

    while(true)
    {
        // ClearScreenBuffer(' ');
        
        // DisplayScreenBuffer();$

        if(GetAsyncKeyState(0x53) >> 7)
            sDown++;
        else
            sDown = 0;

        if(sDown == 1)
        {
            C8_cycle(&emu);

            printf("%s", emu.log);
        }
    }
}
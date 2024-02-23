#pragma once

#define UNHANDLED_OPCODE    { \
                                strcat(c8->log, "Unhandled opcode"); \
                                c8->PC -= 2; \
                            }

void (*instructions_8_last_nibble[16])(struct Chip8* c8);
void (*instructions_E_last_nibble[16])(struct Chip8* c8);
void (*instructions_F_last_byte[256])(struct Chip8* c8);

void OP_0xxx(struct Chip8* c8)
{
    switch(c8->op)
    {
    case 0x00e0:
        strcat(c8->log, "CLS");
        
        memset(&c8->screen[0], false, 64 * 32);

        break;

    case 0x00ee:
        strcat(c8->log, "RET");

        c8->PC = c8->stack[c8->SP];
        c8->SP--;

        break;

    default:
        UNHANDLED_OPCODE

        break;
    }
}

void JP_NNN(struct Chip8* c8)
{
    strcat(c8->log, "JP 0x");
    char buf[4];
    itoa(c8->NNN, &buf[0], 16);
    strcat(c8->log, buf);

    c8->PC = c8->NNN;
}

void CALL_NNN(struct Chip8* c8)
{
    strcat(c8->log, "CALL 0x");
    char buf[4];
    itoa(c8->NNN, &buf[0], 16);
    strcat(c8->log, buf);

    c8->SP++;
    c8->stack[c8->SP] = c8->PC;

    c8->PC = c8->NNN;
}

void SE_Vx_NN(struct Chip8* c8)
{
    strcat(c8->log, "SE V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    char buf_1[2];
    itoa(c8->NN, &buf_1[0], 16);
    strcat(c8->log, buf_1);

    c8->PC += 2 * (c8->V[c8->X] == c8->NN);
}

void SNE_Vx_NN(struct Chip8* c8)
{
    strcat(c8->log, "SNE V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    char buf_1[2];
    itoa(c8->NN, &buf_1[0], 16);
    strcat(c8->log, buf_1);

    c8->PC += 2 * (c8->V[c8->X] != c8->NN);
}

void SE_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "SE V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->PC += 2 * (c8->V[c8->X] == c8->V[c8->Y]);
}

void LD_Vx_NN(struct Chip8* c8)
{
    strcat(c8->log, "LD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    char buf_1[2];
    itoa(c8->NN, &buf_1[0], 16);
    strcat(c8->log, buf_1);
    
    c8->V[c8->X] = c8->NN;
}

void ADD_Vx_NN(struct Chip8* c8)
{
    strcat(c8->log, "ADD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    char buf_1[2];
    itoa(c8->NN, &buf_1[0], 16);
    strcat(c8->log, buf_1);
    
    c8->V[c8->X] += c8->NN;
}

void OP_8xxx(struct Chip8* c8)
{
    void (*op_handler)(struct Chip8* c8) = instructions_8_last_nibble[c8->nibble_0001];
    if(op_handler == NULL)
        UNHANDLED_OPCODE
    else
        (*op_handler)(c8);
}

void LD_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "LD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[c8->X] = c8->V[c8->Y];
}

void OR_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "OR V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[c8->X] |= c8->V[c8->Y];
}

void AND_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "AND V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[c8->X] &= c8->V[c8->Y];
}

void XOR_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "XOR V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[c8->X] ^= c8->V[c8->Y];
}

void ADD_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "ADD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    uint16_t sum = c8->V[c8->X] + c8->V[c8->Y];

    c8->V[c8->X] = (uint8_t)sum;

    c8->V[0xf] = (sum > 0xff);
}

void SUB_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "SUB V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[0xf] = (c8->V[c8->X] > c8->V[c8->Y]);

    c8->V[c8->X] -= c8->V[c8->Y];
}

void SHR_Vx(struct Chip8* c8)
{
    strcat(c8->log, "SHR V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[0xf] = (c8->V[c8->X] & 1);

    c8->V[c8->X] >>= 1;
}

void SUBN_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "SUBN V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[0xf] = (c8->V[c8->Y] > c8->V[c8->X]);

    c8->V[c8->X] = c8->V[c8->Y] - c8->V[c8->X];
}

void SHL_Vx(struct Chip8* c8)
{
    strcat(c8->log, "SHL V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->V[0xf] = (c8->V[c8->X] >> 7);

    c8->V[c8->X] <<= 1;
}

void SNE_Vx_Vy(struct Chip8* c8)
{
    strcat(c8->log, "SNE V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);

    c8->PC += 2 * (c8->V[c8->X] != c8->V[c8->Y]);
}

void LD_I_NNN(struct Chip8* c8)
{
    strcat(c8->log, "LD I, 0x");
    char buf[4];
    itoa(c8->NNN, &buf[0], 16);
    strcat(c8->log, buf);

    c8->I = c8->NNN;
}

void JP_V0_NNN(struct Chip8* c8)
{
    strcat(c8->log, "JP V0, 0x");
    char buf[4];
    itoa(c8->NNN, &buf[0], 16);
    strcat(c8->log, buf);

    c8->PC = c8->NNN + c8->V[0];
}

void RND_Vx_NN(struct Chip8* c8)
{
    strcat(c8->log, "RND V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    char buf_1[2];
    itoa(c8->NN, &buf_1[0], 16);
    strcat(c8->log, buf_1);

    c8->V[c8->X] = (rand() & c8->NN);
}

void DRW_Vx_Vy_N(struct Chip8* c8)
{
    strcat(c8->log, "DRW V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", V");
    itoa(c8->Y, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", 0x");
    itoa(c8->N, &buf, 16);
    strcat(c8->log, &buf);

    uint8_t x = c8->V[c8->X], y = c8->V[c8->Y];

    c8->V[0xf] = 0;

    for(uint8_t i = 0; i < c8->N; i++)
    {
        uint8_t row = c8->memory[c8->I + i];
        for(uint8_t j = 0; j < 8; j++)
        {
            bool pix = ((row >> (7 - j)) & 1);
            bool* screenPix = &c8->screen[((i + y) % 32) * 64 + ((j + x) % 64)];

            if(pix && *screenPix)
                c8->V[0xf] = 1;

            *screenPix ^= pix;
        }
    }
}

void OP_Exxx(struct Chip8* c8)
{
    void (*op_handler)(struct Chip8* c8) = instructions_E_last_nibble[c8->nibble_0001];
    if(op_handler == NULL)
        UNHANDLED_OPCODE
    else
        (*op_handler)(c8);
}

void SKP_Vx(struct Chip8* c8)
{
    strcat(c8->log, "SKP V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->PC += 2 * (GetAsyncKeyState(keymap[c8->V[c8->X]]) >> 7);
}

void SKNP_Vx(struct Chip8* c8)
{
    strcat(c8->log, "SKNP V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->PC += 2 * (!(GetAsyncKeyState(keymap[c8->V[c8->X]]) >> 7));
}

void OP_Fxxx(struct Chip8* c8)
{
    void (*op_handler)(struct Chip8* c8) = instructions_F_last_byte[c8->op & 0xff];
    if(op_handler == NULL)
        UNHANDLED_OPCODE
    else
        (*op_handler)(c8);
}

void LD_Vx_DT(struct Chip8* c8)
{
    strcat(c8->log, "LD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", DT");

    c8->V[c8->X] = c8->DT;
}

void LD_Vx_K(struct Chip8* c8)
{
    strcat(c8->log, "LD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", K");

    bool keyDown = false;
    uint8_t K = 0;
    for(uint8_t i = 0; i < 16; i++)
    {
        if(GetAsyncKeyState(keymap[i]) >> 7)
        {
            keyDown = true;
            K = i;
        }
    }

    if(!keyDown)
        c8->PC -= 2;
    else
        c8->V[c8->X] = K;
}

void LD_DT_Vx(struct Chip8* c8)
{
    strcat(c8->log, "LD DT, V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->DT = c8->V[c8->X];
}

void LD_ST_Vx(struct Chip8* c8)
{
    strcat(c8->log, "LD ST, V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->ST = c8->V[c8->X];
}

void ADD_I_Vx(struct Chip8* c8)
{
    strcat(c8->log, "ADD I, V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->I += c8->V[c8->X];

    c8->I &= 0x0fff; // 12 bits
}

void LD_F_Vx(struct Chip8* c8)
{
    strcat(c8->log, "LD F, V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    c8->I = 5 * c8->V[c8->X];
}

void LD_B_Vx(struct Chip8* c8)
{
    strcat(c8->log, "LD B, V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    uint8_t value = c8->V[c8->X];
    c8->memory[c8->I] =     (value / 100) % 10;
    c8->memory[c8->I + 1] = (value / 10) % 10;
    c8->memory[c8->I + 2] =  value % 10;
}

void LD__I__Vx(struct Chip8* c8)
{
    strcat(c8->log, "LD [I], V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);

    for(uint8_t i = 0; i <= c8->X; i++)
        c8->memory[c8->I + i] = c8->V[i];
}

void LD_Vx__I_(struct Chip8* c8)
{
    strcat(c8->log, "LD V");
    char buf;
    itoa(c8->X, &buf, 16);
    strcat(c8->log, &buf);
    strcat(c8->log, ", [I]");

    for(uint8_t i = 0; i <= c8->X; i++)
        c8->V[i] = c8->memory[c8->I + i];
}

void (*instructions_first_nibble[16])(struct Chip8* c8) = 
{
    OP_0xxx,    JP_NNN,         CALL_NNN,   SE_Vx_NN, 
    SNE_Vx_NN,  SE_Vx_Vy,       LD_Vx_NN,   ADD_Vx_NN, 
    OP_8xxx,    SNE_Vx_Vy,      LD_I_NNN,   JP_V0_NNN, 
    RND_Vx_NN,  DRW_Vx_Vy_N,    OP_Exxx,    OP_Fxxx
};

void (*instructions_8_last_nibble[16])(struct Chip8* c8) = 
{
    LD_Vx_Vy,   OR_Vx_Vy,       AND_Vx_Vy,  XOR_Vx_Vy, 
    ADD_Vx_Vy,  SUB_Vx_Vy,      SHR_Vx,     SUBN_Vx_Vy, 
    NULL,       NULL,           NULL,       NULL, 
    NULL,       NULL,           SHL_Vx,     NULL
};

void (*instructions_E_last_nibble[16])(struct Chip8* c8) = 
{
    NULL,       SKNP_Vx,        NULL,       NULL,  
    NULL,       NULL,           NULL,       NULL, 
    NULL,       NULL,           NULL,       NULL, 
    NULL,       NULL,           SKP_Vx,     NULL
};

void (*instructions_F_last_byte[256])(struct Chip8* c8) = 
{
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       LD_Vx_DT,   NULL,       NULL,       LD_Vx_K,    NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       LD_DT_Vx,   NULL,       NULL,       LD_ST_Vx,   NULL,       NULL,       NULL,       NULL,       NULL,       ADD_I_Vx,   NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       LD_F_Vx,    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       LD_B_Vx,    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       LD__I__Vx,  NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       LD_Vx__I_,  NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       
    NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL,       NULL
};
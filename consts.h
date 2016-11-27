#ifndef CONSTS_H

#include <stdint.h>

#define REG_MAX   0xF
#define MEM_MAX   0xFF
#define BUF_MAX   80
#define LINE_MAX  5
#define PC_MAX    1
#define PC_INIT   0x00
#define FILE_MODE "r"

#define LO_BYTE(x) ((uint8_t)((x)&0xFF)) 
#define HI_BYTE(x) ((uint8_t)(((x)>>8)&0xFF))
#define LO_NIB(x) ((uint8_t)((x)&0xF))
#define HI_NIB(x) ((uint8_t)(((x)>>4)&0xF))

#define OP_N1 HI_NIB(HI_BYTE(*arg->op))
#define OP_N2 LO_NIB(HI_BYTE(*arg->op))
#define OP_N3 HI_NIB(LO_BYTE(*arg->op))
#define OP_N4 LO_NIB(LO_BYTE(*arg->op))
#define OP_B1 HI_BYTE(*arg->op)
#define OP_B2 LO_BYTE(*arg->op)

typedef struct InstArg {
    uint8_t* mem; 
    uint8_t* reg;
    uint8_t* pc;
    uint16_t* const op;
} InstArg;


#endif

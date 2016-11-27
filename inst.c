#include "inst.h"

const char* const OP_V = "--------->";

#define F_MNT(x) (x & 0xF)
#define F_EXP(x) ((x >> 4) & 0x7)
#define F_SGN(x) ((x >> 7) & 0x1)
#define F_MAX 1

bool ldr(InstArg *arg) {
    arg->reg[OP_N2] = arg->mem[OP_B2];
    printf("%sLoading reg 0x%x with *0x%02x (0x%02x)\n",
            OP_V, OP_N2, OP_B2, arg->mem[OP_B2]);
    pci(arg);
    return true;
}

bool ldv(InstArg *arg) {
    arg->reg[OP_N2] = OP_B2; 
    printf("%sLoading reg 0x%x with 0x%02x\n",
            OP_V, OP_N2, OP_B2);
    pci(arg);
    return true;
}

bool sto(InstArg *arg) {
    arg->mem[OP_B2] = arg->reg[OP_N2];
    printf("%sLoading *0x%02x with *reg 0x%x (0x%02x)\n",
            OP_V, OP_B2, OP_N2, arg->reg[OP_N2]); 
    pci(arg);
    return true;
}

bool mov(InstArg *arg) {
    arg->reg[OP_N4] = arg->reg[OP_N3];
    printf("%sLoading reg 0x%x with *reg 0x%x (0x%02x)\n",
            OP_V, OP_N4, OP_N3, arg->reg[OP_N3]);
    pci(arg);
    return true;
}

bool adt(InstArg *arg) {
    printf("%sLoading reg 0x%x with (2's) *reg 0x%x + "
            "*reg 0x%x (0x%02x + 0x%02x)",
            OP_V, OP_N2, OP_N3, OP_N4, arg->reg[OP_N3], 
            arg->reg[OP_N4]);
    arg->reg[OP_N2] = arg->reg[OP_N3] + arg->reg[OP_N4];
    printf("; 0x%02x\n", arg->reg[OP_N2]);
        pci(arg);
    return true;
}

bool adf(InstArg *arg) {
    // remove when done
    puts("Incomplete.");
    return false;

    uint32_t R[F_MAX+1] = {arg->reg[OP_N3], arg->reg[OP_N4]};

    
    printf("SGN1: %x, EXP1: %x, MNT1: %x\n"
           "SGN2: %x, EXP2: %x, MNT2: %x\n",
            F_SGN(R[0]), F_EXP(R[0]), F_MNT(R[0]),
            F_SGN(R[1]), F_EXP(R[1]), F_MNT(R[1]));


    for (int i = 0; i < F_MAX + 1; i++) {
        // convert f_exp to excess
        R[i] = F_MNT(R[i]) >> F_EXP(R[i]);

        uint8_t LSSB;

        for (LSSB = 0x0; LSSB < 0x9; LSSB++) {
            if ((R[i] >> LSSB) & 0x1) {
                printf("bit #%x is set\n", LSSB);
                break;
            }
        }
        if (LSSB == 0x9) {
            puts("Mantissa is 0");
            LSSB = 0;
        }
        printf("LSSB: %x\n", LSSB);
    }
   
    pci(arg);
    return true;
}

bool ior(InstArg *arg) {
    printf("%sLoading reg 0x%x with *reg 0x%x (0x%02x) | "
            "*reg 0x%x (0x%02x)",
            OP_V, OP_N2, OP_N3, arg->reg[OP_N3], OP_N4,
            arg->reg[OP_N4]);
    arg->reg[OP_N2] = arg->reg[OP_N3] | arg->reg[OP_N4]; 
    printf("; 0x%02x\n", arg->reg[OP_N2]);
    pci(arg);
    return true;
}

bool and(InstArg *arg) {
    printf("%sLoading reg 0x%x with *reg 0x%x (0x%02x) & "
            "*reg 0x%x (0x%02x)",
            OP_V, OP_N2, OP_N3, arg->reg[OP_N3], OP_N4,
            arg->reg[OP_N4]);
    arg->reg[OP_N2] = arg->reg[OP_N3] & arg->reg[OP_N4]; 
    printf("; 0x%02x\n", arg->reg[OP_N2]);
        pci(arg);
    return true;
}

bool xor(InstArg *arg) {
    printf("%sLoading reg 0x%x with *reg 0x%x (0x%02x) ^ "
            "*reg 0x%x (0x%02x)",
            OP_V, OP_N2, OP_N3, arg->reg[OP_N3], OP_N4,
            arg->reg[OP_N4]);
    arg->reg[OP_N2] = arg->reg[OP_N3] ^ arg->reg[OP_N4]; 
    printf("; 0x%02x\n", arg->reg[OP_N2]);
    pci(arg);
    return true;
}

bool rot(InstArg *arg) {
    
    printf("%sLoading reg 0x%x with *reg 0x%x (0x%02x) >> 0x%x",
           OP_V, OP_N2, OP_N2, arg->reg[OP_N2], OP_N4); 
    arg->reg[OP_N2] = arg->reg[OP_N2] >> OP_N4; 
    printf("; 0x%02x\n", arg->reg[OP_N2]);
    pci(arg);
    return true;
}

bool jmp(InstArg *arg) {
    if (arg->reg[OP_N2] == arg->reg[0]) {
        printf("%s*Reg 0x%x (0x%02x) equal to *reg 0x%x (0x%02x), "
                "jumping to 0x%02x\n",
                OP_V, OP_N2, arg->reg[OP_N2], 0x0, arg->reg[0x0], OP_B2);
        *arg->pc = OP_B2;
    }
    else {
        printf("%s*Reg 0x%x (0x%02x) not equal to *reg 0x%x (0x%02x), "
                "not jumping to 0x%02x\n",
                OP_V, OP_N2, arg->reg[OP_N2], 0x0, arg->reg[0x0], OP_B2);
        pci(arg);
    }
    return true;
}

bool hlt(InstArg *arg) {
    printf("%sExecution halted\n", OP_V);
    return false;
}

bool pci(InstArg *arg) {
    *arg->pc += 0x2;
    return true;
}

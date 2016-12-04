#ifndef INST_H
#define INST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "consts.h"


bool ldr(InstArg *arg); // [1] load reg by ref
bool ldv(InstArg *arg); // [2] load reg by value
bool sto(InstArg *arg); // [3] load mem by ref
bool mov(InstArg *arg); // [4] copy reg values
bool adt(InstArg *arg); // [5] add (2's) reg 2 and 3, store in reg 1
bool adf(InstArg *arg); // [6] add (float) reg 2 and 3, store in reg 1
bool ior(InstArg *arg); // [7] inc. or reg 2 and 3, store in reg 1
bool and(InstArg *arg); // [8] and reg 2 and 3, store in reg 1
bool xor(InstArg *arg); // [9] exc. or reg 2 and 3, store in reg 1
bool rot(InstArg *arg); // [A] rotate reg given times
bool jmp(InstArg *arg); // [B] jump to mem if given reg == reg 0
bool hlt(InstArg *arg); // [C] stop!

uint32_t adf_conv(uint8_t, uint8_t, uint8_t);
bool pci(InstArg *arg); // increments PC
#endif

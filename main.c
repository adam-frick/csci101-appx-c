#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "inst.h"
#include "consts.h"

uint8_t reg [REG_MAX+1] = {'\0'};
uint8_t mem [MEM_MAX+1] = {'\0'};
uint8_t reg_pc = PC_INIT;

bool load_mem(char* const filename);
bool parse_prog(char* const line);
void init();
bool read();

int main(int argc, char** argv) {
    switch (argc) {
        case 0+1:
            puts("Please provide a filename" 
                   " as an argument.");
            exit(EXIT_FAILURE);
            break;
        case 1+1:
            init(argv);
            while(read());
            break;
        case 2+1:
            puts("Please provide only one filename"
                   " as an argument.");
            exit(EXIT_FAILURE);
            break;
        default:
            puts("Unknown argument error.");
            exit(EXIT_FAILURE);
            break;
    }
    exit(EXIT_SUCCESS);
}

void init(char** argv) {
    if (!load_mem(argv[0+1])) exit(EXIT_FAILURE);
}

bool read() {
    uint16_t inst = (mem[reg_pc] << 8) | (mem[reg_pc+1]);
    bool success = false;

    InstArg arg = (InstArg) {
        .mem = mem,
        .reg = reg,
        .pc = &reg_pc, 
        .op = &inst};

    printf("PC:   0x%02x\nInst: 0x%04x\n", *arg.pc, *arg.op);

    switch(HI_NIB(HI_BYTE(*arg.op))) {
        case 0x1:
            success = ldr(&arg);
            break;
        case 0x2:
            success = ldv(&arg);
            break;
        case 0x3:
            success = sto(&arg);
            break;
        case 0x4:
            success = mov(&arg);
            break;
        case 0x5:
            success = adt(&arg);
            break;
        case 0x6:
            success = adf(&arg);
            break;
        case 0x7:
            success = ior(&arg);
            break;
        case 0x8:
            success = and(&arg);
            break;
        case 0x9:
            success = xor(&arg);
            break;
        case 0xA:
            success = rot(&arg);
            break;
        case 0xB:
            success = jmp(&arg);
            break;
        case 0xC:
            success = hlt(&arg);
            break;
        default:
            puts("Blank instruction");
            success = pci(&arg);
    }

    if (!success) puts("Instruction interrupted.");

    return success;
}

bool load_mem(char* const filename) {
    char buffer[BUF_MAX];
    FILE* const prog = fopen(filename, FILE_MODE);

    if (prog == NULL) {
        puts("Cannot open file.");
        return false;
    }

    while (fgets(buffer, BUF_MAX, prog) != NULL) 
        if (!parse_prog(buffer)) break;

    if (feof(prog))
        printf("EOF\n----------\n");
    else {
        printf("Parse error on %s\b line\n", buffer);
        return false;
    }

    fclose(prog);
    return true;
}

bool parse_prog(char* const line) {
    uint8_t buf_idx = 0;
    uint8_t str_idx = 0;
    uint8_t mem_idx;
    char str_temp [BUF_MAX+1] = {'\0'};
    char pc_temp [PC_MAX+1] = {'\0'};
    uint16_t mem_temp;
    for (; line[buf_idx] != '\n'; buf_idx++) {
        if (isxdigit(line[buf_idx])) {
            str_temp[str_idx++] = line[buf_idx];
        }
    }
    
    if (str_idx > LINE_MAX+1) {
        puts("Invalid line.");
        return false;
    }
    
    printf("Parsed line: %s\n", str_temp);
    strncpy(pc_temp, str_temp, PC_MAX+1);
    mem_idx = (uint8_t)strtol(pc_temp, NULL, 0xF+1);
    mem_temp = (uint16_t)strtol(strdup(str_temp + PC_MAX+1), NULL, 0xF+1);
   
    mem[mem_idx] = HI_BYTE(mem_temp);
    mem[mem_idx+1] = LO_BYTE(mem_temp);

    return true;
}

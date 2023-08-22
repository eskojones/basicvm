#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <fcntl.h>


#define SHORT(h,l) ((h & 0x00ff) << 8) + (l & 0x00ff)
#define HBYTE(i) (i >> 8) & 0xff
#define LBYTE(i) i & 0xff
#define OPCODE(vm, x) vm_get_opcode_from_string(vm, x)


#define F_HALT    0
#define F_GREATER 1
#define F_LESS    2
#define F_EQUAL   3
#define F_ZERO    4
#define F_DATA    5

#define DEBUG

struct VM;

typedef struct {
    uint8_t opcode;
    char name[20];
    void (*func)(struct VM *);
} VM_Op;


struct VM {
    uint16_t pc;            //Program Counter
    uint16_t reg[10];       //Registers
    uint8_t mem[65536];     //Memory for Program and Data
    uint8_t flags[10];      //Status Flags
    VM_Op opcodes[256];     //OpCodes available
    uint8_t opcount;        //Count of OpCodes loaded
};


uint8_t vm_get_opcode_from_string (struct VM *vm, char *opcode);
void vm_load_instruction (struct VM *vm, char *name, void (*fn)(struct VM *));
void vm_init (struct VM *vm);
void vm_load (struct VM *vm, char *program, uint16_t length, uint16_t address);
void vm_step (struct VM *vm);


#endif

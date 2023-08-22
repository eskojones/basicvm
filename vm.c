
#include "vm.h"
#include "instructions.h"






uint8_t vm_get_opcode_from_string (struct VM *vm, char *opcode) {
    for (uint8_t i = 0; i < vm->opcount; i++) {
        if (strcmp(vm->opcodes[i].name, opcode) == 0) return i;
    }
    #ifdef DEBUG
        printf("vm_get_opcode_from_string(%s): not found\n", opcode);
    #endif
    return 0x00; //should be nop or hlt
}


void vm_load_instruction (struct VM *vm, char *name, void (*fn)(struct VM *)) {
    for (uint8_t i = 0; i < vm->opcount; i++) {
        if (strcmp(vm->opcodes[i].name, name) == 0) {
            #ifdef DEBUG
                printf("[!] instruction '%s' already loaded!\n", name);
            #endif
            return;
        }
    }
    VM_Op op = {
        vm->opcount,
        "",
        fn
    };
    snprintf(op.name, 20, "%s", name);
    vm->opcodes[vm->opcount++] = op;    
}


void vm_init (struct VM *vm) {
    #ifdef DEBUG
        printf("[+] initialising vm...\n");
    #endif
    memset(vm, 0, sizeof(struct VM));
    vm_load_instruction(vm, "hlt",     vm_instruction_hlt);
    vm_load_instruction(vm, "nop",     vm_instruction_nop);
    vm_load_instruction(vm, "mov_ir",  vm_instruction_mov_ir);
    vm_load_instruction(vm, "print",   vm_instruction_print);
    vm_load_instruction(vm, "inc",     vm_instruction_inc);
    vm_load_instruction(vm, "dec",     vm_instruction_dec);
    vm_load_instruction(vm, "jmp",     vm_instruction_jmp);
    vm_load_instruction(vm, "cmp_i",   vm_instruction_cmp_i);
    vm_load_instruction(vm, "je",      vm_instruction_je);
    vm_load_instruction(vm, "jne",     vm_instruction_jne);
    vm_load_instruction(vm, "jl",      vm_instruction_jl);
    vm_load_instruction(vm, "jg",      vm_instruction_jg);
    vm_load_instruction(vm, "jle",     vm_instruction_jle);
    vm_load_instruction(vm, "jge",     vm_instruction_jge);
    vm_load_instruction(vm, "jz",      vm_instruction_jz);
    vm_load_instruction(vm, "jnz",     vm_instruction_jnz);
    vm_load_instruction(vm, "add_ir",  vm_instruction_add_ir);
    vm_load_instruction(vm, "sub_ir",  vm_instruction_sub_ir);
    vm_load_instruction(vm, "mul_ir",  vm_instruction_mul_ir);
    vm_load_instruction(vm, "div_ir",  vm_instruction_div_ir);
    vm_load_instruction(vm, "mov_rr",  vm_instruction_mov_rr);
    vm_load_instruction(vm, "mov_mr",  vm_instruction_mov_mr);
    vm_load_instruction(vm, "mov_im",  vm_instruction_mov_im);
    vm_load_instruction(vm, "stdin",   vm_instruction_stdin);
    vm_load_instruction(vm, "add_rr",  vm_instruction_add_rr);
    
}


void vm_load (struct VM *vm, char *program, uint16_t length, uint16_t address) {
    #ifdef DEBUG
        printf("[+] loading program (%d bytes) at 0x%04x...", length, address);
        fflush(stdout);
    #endif
    for (uint16_t i = 0; i < length; i++) {
        vm->mem[address + i] = program[i];
    }
    #ifdef DEBUG
        printf("done.\n");
    #endif
    vm->pc = address;
}


void vm_step (struct VM *vm) {
    if (vm->flags[F_HALT] != 0) return;
    vm->opcodes[vm->mem[vm->pc]].func(vm);
}


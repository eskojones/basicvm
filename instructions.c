#include "vm.h"
#include "instructions.h"

/*
 * No Operation
 * NOP
 */
void vm_instruction_nop (struct VM *vm) {
    vm->pc++;
}

/*
 * Halt Execution
 * HLT
 */
void vm_instruction_hlt (struct VM *vm) {
    #ifdef DEBUG
        printf("0x%04x\t hlt\n", vm->pc);
        printf("[+] execution halted\n");
    #endif

    vm->flags[F_HALT] = 1;
}

/*
 * Move Word (Immediate to Register)
 * MOV_IR <REG_IDX:8> <IMM_VALUE:16>
 * MOV_IR 0x00 0x1337
 */
void vm_instruction_mov_ir (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    int h = vm->mem[vm->pc + 2];
    int l = vm->mem[vm->pc + 3];
    vm->reg[register_index] = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t mov_ir\t (0x%04x) => r%d\n", vm->pc, vm->reg[register_index], register_index);
    #endif
    vm->pc += 4;
}

/*
 * Print character to stdout
 * Prints ASCII character at memory address stored in R0
 * PRINT
 */
void vm_instruction_print (struct VM *vm) {
    char ch = vm->mem[vm->reg[0]];
    #ifdef DEBUG
        printf("0x%04x\t print\t 0x%04x (0x%02x '%c')\n", vm->pc, vm->reg[0], ch, (ch < 32 ? '.' : ch));
    #endif
    #ifndef DEBUG
        putchar(ch);
        fflush(stdout);
    #endif
    vm->pc++;
}

/*
 * Increment (Register)
 * INC <REG_IDX:8>
 * INC 0x00
 */
void vm_instruction_inc (struct VM *vm) {
    int index = vm->mem[vm->pc + 1];
    vm->reg[index]++;
    #ifdef DEBUG
        printf("0x%04x\t inc\t r%d (0x%04x)\n", vm->pc, index, vm->reg[index]);
    #endif
    vm->pc += 2;
}

/*
 * Decrement (Register)
 * DEC <REG_IDX:8>
 * DEC 0x00
 */
void vm_instruction_dec (struct VM *vm) {
    int index = vm->mem[vm->pc + 1];
    vm->reg[index]--;
    #ifdef DEBUG
        printf("0x%04x\t dec\t r%d (0x%04x)\n", vm->pc, index, vm->reg[index]);
    #endif
    vm->pc += 2;
}

/*
 * Jump (Memory, Unconditional)
 * Sets PC to memory address specified
 * JMP <ADDR:16>
 * JMP 0xC4F3
 */
void vm_instruction_jmp (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    vm->pc = address;
    #ifdef DEBUG
        printf("0x%04x\t jmp\t 0x%04x\n", vm->pc, address);
    #endif
}

/*
 * Compare (Immediate)
 * Compares the immediate value in argument with the value in R0
 * This sets the appropriate VM->flags[]
 * CMP_I <IMM_VALUE:16>
 * CMP_I 0x1337
 */
void vm_instruction_cmp_i (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int val = SHORT(h, l);
    vm->flags[F_GREATER] = vm->reg[0] > val ? 1 : 0;
    vm->flags[F_LESS] = vm->reg[0] < val ? 1 : 0;
    vm->flags[F_EQUAL] = val == vm->reg[0];
    vm->flags[F_ZERO] = vm->reg[0] == 0 ? 1 : 0;
    #ifdef DEBUG
        printf(
            "0x%04x\t cmp_i\t (0x%04x <=> 0x%04x): GT=%d LT=%d EQ=%d ZERO=%d\n", 
            vm->pc, vm->reg[0], val,
            vm->flags[F_GREATER], vm->flags[F_LESS], 
            vm->flags[F_EQUAL], vm->flags[F_ZERO]
        );
    #endif
    vm->pc += 3;
}

/*
 * Jump if Equal (Immediate)
 * Sets PC to memory address argument if VM->flags[F_EQUAL] is non-zero
 * JE <ADDR:16>
 * JE 0xC4F3
 */
void vm_instruction_je (struct VM *vm) {
    if (vm->flags[F_EQUAL] == 0) {
        vm->pc += 3;
        return;
    }
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    vm->pc = address;
    #ifdef DEBUG
        printf("0x%04x\t je\t 0x%04x\n", vm->pc, address);
    #endif
}

/*
 * Jump if NOT Equal (Immediate)
 * Inverted JE
 * JNE <ADDR:16>
 * JNE 0xC4F3
 */
void vm_instruction_jne (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jne\t 0x%04x\n", vm->pc, address);
    #endif

    if (vm->flags[F_EQUAL] == 1) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if Less Than (Immediate)
 * Sets PC to memory address argument if VM->flags[F_LESS] is non-zero
 * JL <ADDR:16>
 * JL 0xC4F3
 */
void vm_instruction_jl (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jl\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_LESS] == 0) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if Greater Than (Immediate)
 * Sets PC to memory address argument if VM->flags[F_GREATER] is non-zero
 * JG <ADDR:16>
 * JG 0xC4F3
 */
void vm_instruction_jg (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jg\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_GREATER] == 0) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if Less Than OR Equal (Immediate)
 * Sets PC to memory address argument if VM->flags[F_LESS] or VM->flags[F_EQUAL] is non-zero
 * JLE <ADDR:16>
 * JLE 0xC4F3
 */
void vm_instruction_jle (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jle\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_LESS] == 0 && vm->flags[F_EQUAL] == 0) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if Greater Than OR Equal (Immediate)
 * Sets PC to memory address argument if VM->flags[F_GREATER] or VM->flags[F_EQUAL] is non-zero
 * JGE <ADDR:16>
 * JGE 0xC4F3
 */
void vm_instruction_jge (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jge\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_GREATER] == 0 && vm->flags[F_EQUAL] == 0) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if Zero (Immediate)
 * Sets PC to memory address argument if VM->flags[F_ZERO] is non-zero
 * JZ <ADDR:16>
 * JZ 0xC4F3
 */
void vm_instruction_jz (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jz\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_ZERO] == 0) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Jump if NOT Zero (Immediate)
 * Inverted JZ
 * JNZ <ADDR:16>
 * JNZ 0xD34D
 */
void vm_instruction_jnz (struct VM *vm) {
    int h = vm->mem[vm->pc + 1];
    int l = vm->mem[vm->pc + 2];
    int address = SHORT(h, l);
    #ifdef DEBUG
        printf("0x%04x\t jnz\t 0x%04x\n", vm->pc, address);
    #endif
    if (vm->flags[F_ZERO] == 1) {
        vm->pc += 3;
        return;
    }
    vm->pc = address;
}

/*
 * Add (Immediate to Register)
 * Adds the immediate value to the value in the register specified
 * ADD_IR <REG_IDX:8> <IMM_VALUE:16>
 * ADD_IR R0, 0xBEEF
 */
void vm_instruction_add_ir (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    uint16_t val = SHORT(vm->mem[vm->pc + 2], vm->mem[vm->pc + 3]);
    uint16_t reg_val = vm->reg[register_index];
    vm->reg[0] = reg_val + val;
    #ifdef DEBUG
        printf("0x%04x\t add_ir\t r%d (0x%04x) + (0x%04x) = 0x%04x\n", vm->pc, register_index, reg_val, val, vm->reg[0]);
    #endif
    vm->pc += 4;
}


/*
 * Subtract (Immediate to Register)
 * Subtracts the immediate value from the value in the register specified
 * SUB_IR <REG_IDX:8> <IMM_VALUE:16>
 * SUB_IR R0, 0xDEED
 */
void vm_instruction_sub_ir (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    uint16_t val = SHORT(vm->mem[vm->pc + 2], vm->mem[vm->pc + 3]);
    uint16_t reg_val = vm->reg[register_index];
    vm->reg[0] = reg_val - val;
    #ifdef DEBUG
        printf("0x%04x\t sub_ir\t r%d (0x%04x) - (0x%04x) = 0x%04x\n", vm->pc, register_index, reg_val, val, vm->reg[0]);
    #endif
    vm->pc += 4;
}

/*
 * Multiply (Immediate to Register)
 * Multiplies the value in the register specified by the immediate value
 * MUL_IR <REG_IDX:8> <IMM_VALUE:16>
 * MUL_IR R0, 0x0004
 */
void vm_instruction_mul_ir (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    uint16_t val = SHORT(vm->mem[vm->pc + 2], vm->mem[vm->pc + 3]);
    uint16_t reg_val = vm->reg[register_index];
    vm->reg[0] = reg_val * val;
    #ifdef DEBUG
        printf("0x%04x\t mul_ir\t r%d (0x%04x) * (0x%04x) = 0x%04x\n", vm->pc, register_index, reg_val, val, vm->reg[0]);
    #endif
    vm->pc += 4;
}

/*
 * Divide (Register by Immediate)
 * TODO: Implement properly with remainder
 * DIV_IR <REG_IDX:8> <IMM_VALUE:16>
 * DIV_IR R3, 0x000A
 */
void vm_instruction_div_ir (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    uint16_t val = SHORT(vm->mem[vm->pc + 2], vm->mem[vm->pc + 3]);
    uint16_t reg_val = vm->reg[register_index];
    vm->reg[0] = vm->reg[register_index] / val;
    #ifdef DEBUG
        printf("0x%04x\t div_ir\t r%d (0x%04x) / (0x%04x) = 0x%04x\n", vm->pc, register_index, reg_val, val, vm->reg[0]);
    #endif
    vm->pc += 4;
}

/*
 * Move (Register to Register)
 * Moves the value in Register A to Register B
 * MOV_RR <REG_IDX:8> <REG_IDX:8>
 * MOV_RR R1, R3
 */
void vm_instruction_mov_rr (struct VM *vm) {
    int r_a = vm->mem[vm->pc + 1];
    int r_b = vm->mem[vm->pc + 2];
    vm->reg[r_a] = vm->reg[r_b];
    #ifdef DEBUG
        printf("0x%04x\t mov_rr\t r%d (0x%04x) => r%d\n", vm->pc, r_b, vm->reg[r_b], r_a);
    #endif
    vm->pc += 3;
}

/*
 * Move (Memory to Register)
 * Moves the value at the memory address specified into the register specified
 * MOV_MR <REG_IDX:8> <ADDR:16>
 * MOV_MR R2, 0xA000
 */
void vm_instruction_mov_mr (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    int address = SHORT(vm->mem[vm->pc + 2], vm->mem[vm->pc + 3]);
    vm->reg[register_index] = (vm->mem[address] << 8) + vm->mem[address + 1];
    #ifdef DEBUG
        printf("0x%04x\t mov_mr\t 0x%04x (0x%04x) => r%d\n", vm->pc, address, vm->reg[register_index], register_index);
    #endif
    vm->pc += 4;
}

/*
 * Move (Immediate to Memory)
 * Moves the immediate value specified into the memory address specified
 * MOV_IM <ADDR:16> <IMM_VALUE:16>
 * MOV_IM 0xD00D, 0x1337
 */
void vm_instruction_mov_im (struct VM *vm) {
    int address = SHORT(vm->mem[vm->pc + 1], vm->mem[vm->pc + 2]);
    vm->mem[address    ] = vm->mem[vm->pc + 3];
    vm->mem[address + 1] = vm->mem[vm->pc + 4];
    #ifdef DEBUG
        printf("0x%04x\t mov_im\t (0x%02x%02x) => 0x%04x\n", vm->pc, vm->mem[vm->pc + 3], vm->mem[vm->pc + 4], address);
    #endif
    vm->pc += 5;
}

/*
 * Move (Register to Memory)
 * Moves the value in register to the memory address specified
 * MOV_RM <ADDR:16> <REG_IDX:8>
 * MOV_RM 0x4000, R0
 */
void vm_instruction_mov_rm (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 3];
    int address = SHORT(vm->mem[vm->pc + 1], vm->mem[vm->pc + 2]);
    int reg_val = vm->reg[register_index];
    vm->mem[address    ] = HBYTE(reg_val);
    vm->mem[address + 1] = LBYTE(reg_val);
    #ifdef DEBUG
        printf("0x%04x\t mov_rm\t r%d (0x%04x) => 0x%04x\n", vm->pc, register_index, vm->reg[register_index], address);
    #endif
    vm->pc += 4;
}

/*
 * Standard Input (Poll)
 * Polls for a character from standard input and puts the byte (if any) in the register specified
 * INPUT <REG_IDX:8>
 * INPUT R4
 */
void vm_instruction_stdin (struct VM *vm) {
    int register_index = vm->mem[vm->pc + 1];
    char ch = 0x0;
    fcntl(0, F_SETFL, O_NONBLOCK);
    int byte_read = read(0, &ch, 1);
    fcntl(0, F_SETFL, ~O_NONBLOCK);
    if (byte_read > 0) {
        vm->reg[register_index] = ch;
        vm->flags[F_ZERO] = 0;
        #ifdef DEBUG
            printf("0x%04x\t stdin\t %d (0x%02x) => r%d\n", vm->pc, byte_read, ch, register_index);
        #endif
    } else {
        vm->flags[F_ZERO] = 1;
    }
    
    vm->pc += 2;
}

void vm_instruction_add_rr (struct VM *vm) {
    int reg_dst = vm->mem[vm->pc + 1];
    int reg_src = vm->mem[vm->pc + 2];
    uint16_t sum = vm->reg[reg_dst] + vm->reg[reg_src];    
    #ifdef DEBUG
        printf("0x%04x\t add_rr\t r%d (0x%04x) + r%d (0x%04x) => r%d (0x%04x)\n", vm->pc, reg_dst, vm->reg[reg_dst], reg_src, vm->reg[reg_src], reg_dst, sum);
    #endif
    vm->reg[reg_dst] = sum;
    vm->pc += 3;
}

/*
 * I/O Configure (PICO GPIO Pins)
 * Placeholder for configuring GPIO pins (in/out, pull up/down)
 * IOCFG <pin:8> <dir(0|1)> <pullup(0|1)>
 */
void vm_instruction_iocfg (struct VM *vm) {
    vm->pc++;
}

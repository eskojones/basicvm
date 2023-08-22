#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include "vm.h"

void vm_instruction_nop (struct VM *vm);
void vm_instruction_hlt (struct VM *vm);
void vm_instruction_mov_ir (struct VM *vm);
void vm_instruction_print (struct VM *vm);
void vm_instruction_inc (struct VM *vm);
void vm_instruction_dec (struct VM *vm);
void vm_instruction_jmp (struct VM *vm);
void vm_instruction_cmp_i (struct VM *vm);
void vm_instruction_je (struct VM *vm);
void vm_instruction_jne (struct VM *vm);
void vm_instruction_jl (struct VM *vm);
void vm_instruction_jg (struct VM *vm);
void vm_instruction_jle (struct VM *vm);
void vm_instruction_jge (struct VM *vm);
void vm_instruction_jz (struct VM *vm);
void vm_instruction_jnz (struct VM *vm);
void vm_instruction_add_ir (struct VM *vm);
void vm_instruction_sub_ir (struct VM *vm);
void vm_instruction_mul_ir (struct VM *vm);
void vm_instruction_div_ir (struct VM *vm);
void vm_instruction_mov_rr (struct VM *vm);
void vm_instruction_mov_mr (struct VM *vm);
void vm_instruction_mov_im (struct VM *vm);
void vm_instruction_mov_rm (struct VM *vm);
void vm_instruction_stdin (struct VM *vm);
void vm_instruction_add_rr (struct VM *vm);
void vm_instruction_iocfg (struct VM *vm);

#endif

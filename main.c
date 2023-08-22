#include <stdio.h>
#include <stdlib.h>

#include "vm.h"


int main (int argc, char **argv) {
    struct VM vm;
    vm_init(&vm);
    
    //Hello World
    char hello_code[] = {
        //move the address of our data into R0
        //print (which will print the address at R0)
        //increment R0
        //compare R0 and 0x010e (0x0100 + 14, the length of our data)
        //jump to 0x0204 if R0 is less than 0x010e
        OPCODE(&vm, "mov_ir"),   0x00, 0x01, 0x00, //mov_ir r0, 0x0100
        OPCODE(&vm, "print"),                      //<-- addr 0x0204
        OPCODE(&vm, "inc"),      0x00,             //inc r0
        OPCODE(&vm, "cmp_i"),    0x01, 0x0e,       //cmp_i 0x010e
        OPCODE(&vm, "jl"),       0x02, 0x04,       //jl 0x0204
        OPCODE(&vm, "hlt")       
    };
    char hello_data[] = "Hello, World!\n";
    int hello_length = sizeof(hello_code);
    
    //vm_load() sets PC so load data first, then code last
    vm_load(&vm, hello_data, strlen(hello_data), 0x0100); //put our data segment at 0x0100
    vm_load(&vm, hello_code, hello_length, 0x0200);       //and the code at 0x0200
    
    while(vm.flags[F_HALT] == 0) {
        vm_step(&vm);
    }

    printf("\r\n");
    return 0;
}


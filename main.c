#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "interrupts.h"


int main (int argc, char **argv) {
    struct VM vm;
    vm_init(&vm);
    
    char test_program[] = {
        OPCODE(&vm, "nop", ' ', ' '),
        OPCODE(&vm, "mov", 'i', 'r'),    0, 0x12, 0x34,    //mov R0, 0x1234
        OPCODE(&vm, "mov", 'r', 'm'),    0x55, 0x22, 0,    //mov 0x5522, R0
        OPCODE(&vm, "mov", 'm', 'r'),    1, 0x55, 0x22,    //mov R1, [0x5522]
        OPCODE(&vm, "mov", 'p', 'r'),    0, 0x55, 0x22,    //mov R0, [[0x5522]]
        OPCODE(&vm, "mov", 'r', 'r'),    2, 1,
        OPCODE(&vm, "stdout", ' ', ' '), 
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_INFO,
        OPCODE(&vm, "cmp", 'i', ' '),    0x55, 0x22,       //cmp 0x5522<=>R0
        OPCODE(&vm, "cmp", 'r', ' '),    0,                //cmp R0<=>R0
        OPCODE(&vm, "mov", 'i', 'r'),    0, 0x11, 0x22,    //mov R0, 0x1122
        OPCODE(&vm, "mov", 'i', 'r'),    4, 0x00, 0x02,    //mov R0, 0x0002
        OPCODE(&vm, "mov", 'i', 'r'),    5, 0x33, 0x44,    //mov R5, 0x3344
        OPCODE(&vm, "inc", 'r', ' '),    0,                //inc R0
        OPCODE(&vm, "dec", 'r', ' '),    0,                //dec R0
        OPCODE(&vm, "add", 'r', ' '),    5,                //add R0, R5
        OPCODE(&vm, "sub", 'r', ' '),    5,                //sub R0, R5
        OPCODE(&vm, "mul", 'r', ' '),    4,                //mul R0, R4
        OPCODE(&vm, "div", 'r', ' '),    4,                //div R0, R4
        OPCODE(&vm, "shl", 'r', ' '),    0,                //shl R0
        OPCODE(&vm, "shr", 'r', ' '),    0,                //shr R0
        //interrupts-----------------
        //GPIO Configure (pin, direction, pullup/pulldown)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x04, //pin
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 0x01, //dir
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0x00, 0x01, //pullup/pulldown
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_GPIO_CFG,
        //GPIO Set (pin, level)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x04, //pin
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 0x01, //level
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_GPIO_SET,
        //GPIO Get (pin)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x04, //pin
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_GPIO_GET,
        //VIDEO Put Pixel (x, y, colour)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x12, //x
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 0x34, //y
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0xf0, 0x0f, //colour
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_PUTPIXEL,
        //VIDEO Get Pixel (x, y)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x12, //x
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 0x34, //y
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_GETPIXEL,
        //VIDEO Fill (x, y, width, height, colour)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 0x00, //x
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 0x00, //y
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0x00, 160,  //width
        OPCODE(&vm, "mov", 'i', 'r'),    4, 0x00, 128,  //height
        OPCODE(&vm, "mov", 'i', 'r'),    5, 0xff, 0xff, //colour
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_FILL,
        //VIDEO Line (sx, sy, dx, dy, colour)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 10, //sx
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 10, //sy
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0x00, 40, //dx
        OPCODE(&vm, "mov", 'i', 'r'),    4, 0x00, 50, //dy
        OPCODE(&vm, "mov", 'i', 'r'),    5, 0x0f, 0xf0, //colour
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_LINE,
        //VIDEO Circle (cx, cy, radius, colour)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 10, //cx
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 10, //cy
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0x00, 40, //radius
        OPCODE(&vm, "mov", 'i', 'r'),    4, 0x7f, 0xf7, //colour
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_CIRCLE,
        //VIDEO Print (x, y, character, colour)
        OPCODE(&vm, "mov", 'i', 'r'),    1, 0x00, 50, //x
        OPCODE(&vm, "mov", 'i', 'r'),    2, 0x00, 50, //y
        OPCODE(&vm, "mov", 'i', 'r'),    3, 0x00, 'A', //character
        OPCODE(&vm, "mov", 'i', 'r'),    4, 0xff, 0xff, //colour
        OPCODE(&vm, "int", 'i', ' '),    0x00, I_VIDEO_PRINT,
        //---------------------------
        OPCODE(&vm, "hlt", ' ', ' ')
    };
    vm_load(&vm, test_program, sizeof(test_program), 0x0200);
    
    while(vm.flags[F_HALT] == 0) {
        vm_step(&vm);
    }

    printf("\r\n");
    return 0;
}


#include <stdio.h>
#include "interrupts.h"


uint8_t vm_int_info (struct VM *vm) {
    printf("DCM64 https://github.com/eskojones/basicvm\n");
    return 0;
}

uint8_t vm_int_gpio_cfg (struct VM *vm) {
    uint8_t pin = LBYTE(vm->reg[1]);
    uint8_t dir = LBYTE(vm->reg[2]);
    uint8_t pullup = LBYTE(vm->reg[3]);
    printf("INT_GPIO_CFG: NYI (pin:%d, dir:%d, pullup:%d)\n", pin, dir, pullup);
    return 0;
}

uint8_t vm_int_gpio_set (struct VM *vm) {
    uint8_t pin = LBYTE(vm->reg[1]);
    uint8_t level = LBYTE(vm->reg[2]);
    printf("INT_GPIO_SET: NYI (pin:%d, level:%d)\n", pin, level);
    return 0;
}

uint8_t vm_int_gpio_get (struct VM *vm) {
    uint8_t pin = LBYTE(vm->reg[1]);
    printf("INT_GPIO_GET: NYI (pin:%d)\n", pin);
    return 0;
}

uint8_t vm_int_video_putpixel (struct VM *vm) {
    uint16_t x = vm->reg[1];
    uint16_t y = vm->reg[2];
    uint16_t colour = vm->reg[3];
    printf("INT_VIDEO_PUTPIXEL: NYI (x:%d, y:%d, colour:0x%04x)\n", x, y, colour);
    return 0;
}

uint8_t vm_int_video_getpixel (struct VM *vm) {
    uint16_t x = vm->reg[1];
    uint16_t y = vm->reg[2];
    printf("INT_VIDEO_GETPIXEL: NYI (x:%d, y:%d)\n", x, y);
    return 0;
}

uint8_t vm_int_video_fill (struct VM *vm) {
    uint16_t x = vm->reg[1];
    uint16_t y = vm->reg[2];
    uint16_t width = vm->reg[3];
    uint16_t height = vm->reg[4];
    uint16_t colour = vm->reg[5];
    printf("INT_VIDEO_FILL: NYI (x:%d, y:%d, w:%d, h:%d, colour:0x%04x)\n", 
        x, y, width, height, colour
    );
    return 0;
}

uint8_t vm_int_video_line (struct VM *vm) {
    uint16_t sx = vm->reg[1];
    uint16_t sy = vm->reg[2];
    uint16_t dx = vm->reg[3];
    uint16_t dy = vm->reg[4];
    uint16_t colour = vm->reg[5];
    printf("INT_VIDEO_LINE: NYI (sx:%d, sy:%d, dx:%d, dy:%d, colour:0x%04x)\n", 
        sx, sy, dx, dy, colour
    );
    return 0;
}

uint8_t vm_int_video_circle (struct VM *vm) {
    uint16_t cx = vm->reg[1];
    uint16_t cy = vm->reg[2];
    uint16_t radius = vm->reg[3];
    uint16_t colour = vm->reg[4];
    printf("INT_VIDEO_CIRCLE: NYI (cx:%d, cy:%d, radius:%d, colour:0x%04x)\n", 
        cx, cy, radius, colour
    );
    return 0;
}

uint8_t vm_int_video_print (struct VM *vm) {
    uint16_t x = vm->reg[1];
    uint16_t y = vm->reg[2];
    uint16_t ch = vm->reg[3];
    uint16_t colour = vm->reg[4];
    printf("INT_VIDEO_PRINT: NYI (x:%d, y:%d, ch:'%c', colour:0x%04x)\n", 
        x, y, ch, colour
    );
    return 0;
}

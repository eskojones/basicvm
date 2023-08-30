## basicvm
scuffed virtual machine for fun
...expect all instructions to have breaking changes.


## dependencies (if building for pico)
* cmake
* gcc (arm-none-eabi)
* [pico-sdk](https://github.com/raspberrypi/pico-sdk)
* [pico-lcd-base](https://github.com/eskojones/pico-lcd-base)


## build
* copy **/path/to/pico-sdk/external/pico_sdk_import.cmake** into project root
* edit **CMakeLists.txt** and ensure **PICO_LCD_BASE_DIR** is correct
* make a **build** directory in project root and *cd* to it
* *cmake --fresh ..*
* *make*
* copy **build/basicvm.uf2** to your Pico


## license
 MIT
 

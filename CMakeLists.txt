cmake_minimum_required(VERSION 3.12)
### Basic VM + pico-lcd-base
###

##############################
### If using pico-lcd-base...
include(pico_sdk_import.cmake)
set(PICO_BOARD pico) #...or pico_w if you need it
project(basicvm)
pico_sdk_init()
### or, If NOT using pico-lcd-base...
#project(BasicVM)
##############################


### Add include directories here...
### e.g.. include_directories(./includes/mylibrary)
include_directories(./include)



### Define the output executable...
add_executable(
    basicvm
    main.c
    vm.c
    instructions.c
    interrupts.c
)


### BasicVM Debug flags
add_definitions( -DDEBUG )
add_definitions( -DDEBUG_OP )
#add_definitions( -DDEBUG_FLAGS )
#add_definitions( -DDEBUG_REG )


##############################
### If using pico-lcd-base...
add_definitions( -DPICO_LCD_BASE )
add_library(pico_lcd_base STATIC IMPORTED)
### Set path to your static lib of pico_lcd_base and the include path
set_target_properties(
    pico_lcd_base
    PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../pico-lcd-base/build/libpico_lcd_base.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../pico-lcd-base/include"
)
pico_enable_stdio_usb(basicvm 1)
pico_enable_stdio_uart(basicvm 1)
pico_add_extra_outputs(basicvm)
target_link_libraries(
    basicvm 
    pico_lcd_base 
    pico_stdlib 
    pico_time 
    hardware_timer 
    hardware_pwm 
    hardware_i2c 
    hardware_spi 
    hardware_adc
)
##############################
### If not...
#target_link_libraries(basicvm)
##############################





### Make a 'build' directory and run `cmake ..` from inside it.
### Compile and link project with `make -j9` from the 'build' directory.

### *** If CMake is failing under WSL ***
### - Edit /etc/wsl.conf and put this in it:
###   [automount]
###   options = "metadata"
###   enabled = true
### - Close WSL shells and run `wsl -t Debian` from an admin command prompt.
###   Alternatively, replace 'Debian' with 'Ubuntu'.

### *** Compilation under WSL may give 'clock skew' warnings, you can ignore these.

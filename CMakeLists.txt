cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(gd32_template_clion_led C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(MCPU cortex-m4#[[TODO kernel type, ex cortex-m4]])

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork -specs=nosys.specs)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

#             可修改
include_directories(
        Firmware/CMSIS
        Firmware/CMSIS/GD/GD32F4xx/Include
        Firmware/GD32F4xx_standard_peripheral/Include
        Hardware/
        Hardware/cst816
        Hardware/iic
        Hardware/key
        Hardware/LED
        Hardware/lvgl_anim
        Hardware/lvgl_event
        Hardware/lvgl_layout
        Hardware/lvgl_scrolling
        Hardware/lvgl_style
        Hardware/lvgl_widgets
        Hardware/pid_ui
        Hardware/st7789
        Hardware/timer
        Hardware/usart0
        Middlewares/LVGL/GUI/
        Middlewares/LVGL/GUI/lvgl
        Middlewares/LVGL/GUI/lvgl/src
        Middlewares/LVGL/GUI/lvgl/examples/porting
        Middlewares/LVGL/GUI_APP/demos
        User
        )

add_definitions(
        -DGD32F470
        -DUSE_STDPERIPH_DRIVER
        -DNDEBUG
)

file(GLOB_RECURSE SOURCES
        "startup/startup_gd32f450.S"
#        "Firmware/CMSIS/GD/GD32F4xx/Source/ARM/startup_gd32f407_427.s"
        "Firmware/CMSIS/GD/GD32F4xx/Source/*.c"
        "Firmware/GD32F4xx_standard_peripheral/Source/*.c"
        "Hardware/cst816/*.c"
        "Hardware/iic/*.c"
        "Hardware/key/*.c"
        "Hardware/LED/*.c"
        "Hardware/lvgl_anim/*.c"
        "Hardware/lvgl_event/*.c"
        "Hardware/lvgl_layout/*.c"
        "Hardware/lvgl_scrolling/*.c"
        "Hardware/lvgl_style/*.c"
        "Hardware/lvgl_widgets/*.c"
        "Hardware/pid_ui/*.c"
        "Hardware/st7789/*.c"
        "Hardware/timer/*.c"
        "Hardware/usart0/*.c"
        "Middlewares/LVGL/GUI/lvgl/src/*.c"
        "Middlewares/LVGL/GUI/lvgl/examples/porting/*.c"
        "Middlewares/LVGL/GUI_APP/demos/*.c"
        "User/*.*"
        )
# 可修改结束

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/gd32f450z_flash.ld)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork --specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Old_Dart_Stand)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        FR_tasks/Inc/DM_Motor_task.h
        FR_tasks/Src/DM_Motor_task.c
        Others/Inc/RC_remoto_control.h
        FR_tasks/Src/BJ_Motor_task.c
        FR_tasks/Inc/BJ_Motor_task.h
        My_Other/Inc/darts.h
        My_Other/Src/darts.c
        FR_tasks/Inc/Motor_return_zero.h
        FR_tasks/Src/Motor_return_zero.c
        FR_tasks/Inc/Windmill_2006_task.h
        FR_tasks/Src/Windmill_4310_task.c
        FR_tasks/Inc/Windmill_4310_task.h
        FR_tasks/Src/Windmill_2006_task.c)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
        CAN/Src/drv_can.c
        DM_Motor/Src/DM_Motor.c
        FR_tasks/Src/DM_Motor_task.c
        FR_tasks/Src/BJ_Motor_task.c
        FR_tasks/Src/Motor_return_zero.c
        BJ_Motor/Src/Can_BJ.c
        BJ_Motor/Src/X_V2.c
        My_Other/Src/darts.c
        FR_tasks/Src/Windmill_2006_task.c
        FR_tasks/Src/Windmill_4310_task.c
        PID/Src/PID.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        CAN/Inc
        DM_Motor/Inc
        FR_tasks/Inc
        Others/Inc
        BJ_Motor/Inc
        My_Other/Inc
        PID/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

cmake_minimum_required(VERSION 3.10)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

SET(C_FLAGS "-g -gdwarf-2")
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Og -lX11 -lGL -lpthread -lpng -lstdc++fs")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-Map=${PROJECT_NAME}.map")
set(sources
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/src/Bus.cpp
        ${CMAKE_SOURCE_DIR}/src/cpu6502.cpp
        ${CMAKE_SOURCE_DIR}/src/Cartridge.cpp
        ${CMAKE_SOURCE_DIR}/src/Ppu.cpp
        ${CMAKE_SOURCE_DIR}/src/Mapper.cpp)

include_directories(inc)
add_executable(NES_emulator ${sources})
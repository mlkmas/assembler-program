cmake_minimum_required(VERSION 3.22)
project(untitled6 C)

set(CMAKE_C_STANDARD 90)

add_executable(untitled6 src/assembler.c include/constants.h include/utils.h src/utils.c src/firstPass.c include/firstPass.h src/preAssembler.c include/preAssembler.h include/assembler.h)

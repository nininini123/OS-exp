cmake_minimum_required(VERSION 3.8)
project(copy)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES1 main.c buffer.h)
#set(SOURCE_FILES2 readFile.c)
#set(SOURCE_FILES3 writeFile.c)


#add_executable(readFile ${SOURCE_FILES2})
#add_executable(writeFile ${SOURCE_FILES3})
add_executable(copy ${SOURCE_FILES1})

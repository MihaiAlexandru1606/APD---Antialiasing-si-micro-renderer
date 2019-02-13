cmake_minimum_required(VERSION 3.12)
project(Tema_1_APD C)

set(CMAKE_C_STANDARD 11)

#pentru bibliotecatica de matematica si thread
set(GCC_LIB "-lm -lpthread -Wall -lm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_LIB}")


include_directories(.)

find_package (Threads)

add_executable(Tema_1_APD
        homework.c
        homework.h
        homework1.c
        homework1.h
        main.c
        main1.c
        )
target_link_libraries (Tema_1_APD ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Tema_1_APD m)

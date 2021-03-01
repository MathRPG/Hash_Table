cmake_minimum_required(VERSION 3.17)
project(Hash_Table C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(OldHashTable
        include/espalhamento.h
        src/espalhamento.c
        src/oldmain.c)

add_executable(NewHashTable
        include/hashtable.h
        src/hashtable.c
        src/main.c src/article.c src/article.h)

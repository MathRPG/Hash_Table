cmake_minimum_required(VERSION 3.17)
project(Hash_Table C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(HashTable
        src/main.c src/hashtable.c src/article.c)

cmake_minimum_required(VERSION 3.17)
project(Hash_Table C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(HashTableTests
        src/tests.c src/hashtable.c src/article.c)

add_executable(HashTableDemonstration
        src/demonstration.c src/hashtable.c src/article.c)

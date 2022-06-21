cmake_minimum_required(VERSION 3.20)
project(lock_table_final C)

set(CMAKE_C_STANDARD 99)
add_subdirectory(lock-table)
add_executable(lock_table_final main.c)
target_link_libraries(lock_table_final PUBLIC lock_table)

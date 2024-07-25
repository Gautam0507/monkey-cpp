cmake_minimum_required(VERSION 3.30)
project(monkey LANGUAGES CXX)

# Setting cxx min value  
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We are including CTest
include(CTest)

# Downloaded direct version of google test and includeing it 
add_subdirectory(./libraries/googletest-1.14.0)

# Adding all subdirectories 
add_subdirectory(./lexer)
add_subdirectory(./token)
add_subdirectory(./repl)
add_subdirectory(./parser)
add_subdirectory(./ast)

# Commands for linking and compiling main.cpp 
add_executable(monkey main.cpp)
target_include_directories(monkey PRIVATE ./repl)
target_link_libraries(monkey repl)

# Monkey Programming Language Compiler

## Introduction

This project is an implementation of a compiler for the Monkey Toy Programming Language, based on the book "Crafting Interpreters" by Thorsten Ball. The goal of this project is to understand the inner workings of a compiler by building one from scratch.

## Current Progress

I have currently completed the Lexer Parser section of the compiler and am working on the Evaluation part.

## Project Structure

- **AST**: Contains the classes for the Abstract Syntax Tree (AST) nodes.
- **Lexer**: Tokenizes the input source code.
- **Parser**: Analyzes the tokenized input to generate an Abstract Syntax Tree (AST).
- **Tokens**: Represents the different types of tokens in the Monkey language.
- **repl**: Contains the Read-Eval-Print Loop (REPL) for the Monkey language.

## Getting Started

Download the release from the releases section and run the executable.

### Prerequisites

To build and run this project, you need:

- A C++ compiler (GCC, Clang, MSVC, etc.)
- CMake (optional, for easier build configuration)

### Building the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/Gautam0507/monkey-cpp.git

   cd monkey-cpp
   ```

2. Build the project using your preferred method:

   **Using CMake:**

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   **Using a direct compiler command:**

   ```bash
   g++ -o monkey main.cpp lexer.cpp parser.cpp
   ```

### Running the Compiler

After building the project, you can run the compiler using:

```bash
./monkey
```

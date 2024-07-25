# Monkey Programming Language Compiler

## Introduction

This project is an implementation of a compiler for the Monkey Toy Programming Language, based on the book "Crafting Interpreters" by Thorsten Ball. The goal of this project is to understand the inner workings of a compiler by building one from scratch.

## Current Progress

I have currently completed the Lexer Parser section of the compiler and am working on the Evaluation part.
The parser is currently working with operator precedence.

## Project Structure

- **AST**: Contains the classes for the Abstract Syntax Tree (AST) nodes.
- **Lexer**: Tokenizes the input source code.
- **Parser**: Analyzes the tokenized input to generate an Abstract Syntax Tree (AST).
- **Tokens**: Represents the different types of tokens in the Monkey language.
- **repl**: Contains the Read-Eval-Print Loop (REPL) for the Monkey language.

## Getting Started

### Running the Compiler

Download the release from the releases section and run the executable or build the project using the instructions below.

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

Note: need C++ 17 to compile the project

**Using CMake:**

```bash
mkdir build
cd build
cmake ..
make
```

**Using a direct compiler command:**

```bash
g++ -std=c++17 main.cpp ast/ast.cpp lexer/lexer.cpp parser/parser.cpp repl/repl.cpp token/token.cpp
./a.out
```

### Running the Compiler

After building the project, you can run the compiler using:

```bash
./monkey
```

## Commands to test out the working of the compiler

```
let x = 5;
let y = 10;
fn(a,b)
let x = 1 * 2 * 3 * 4 * 5
x * y / 2 + 3 * 8 - 123
true == false
if (x ==5) {return 5;}
if (x == 5) {return 5;} else {return 10;}
let x = fn(a,b) {return a + b;}
```

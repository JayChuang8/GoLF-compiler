# GoLF-compiler

This project is a comprehensive effort to build a compiler for a subset of the Go programming language into MIPS assembly code. The GoLF compiler includes several essential components such as a scanner, a parser that generates an abstract syntax tree (AST), a semantic checker, and a MIPS code generator. The goal of this project is to demonstrate a deep understanding of compiler construction, from lexical analysis to code generation, in the context of the Go programming language.

## Components
### 1. Scanner 
The scanner is responsible for the lexical analysis of Go source code. It breaks down the source code into a stream of tokens, which serve as the fundamental building blocks for the subsequent phases of the compiler. It identifies keywords, identifiers, literals, operators, and other language constructs.

### 2. Parser 
The parser takes the stream of tokens generated by the scanner and constructs an abstract syntax tree (AST) representing the structure and semantics of the Go program. It enforces the grammar rules of the Go language and organizes the code into a hierarchical tree structure.

### 3. Semantic Checker 
The semantic checker performs various checks on the AST to ensure that the Go program is semantically correct. It verifies type compatibility, variable scoping, function declarations, and other aspects of program correctness. Any errors or inconsistencies are reported at this stage.

### 4. MIPS Code Generator
The MIPS code generator translates the validated AST into MIPS assembly code suitable for execution on an x86-64 architecture. It maps Go constructs to equivalent assembly instructions, optimizing where possible to produce efficient code.

## Getting Started
To compile and use this C++ compiler, follow these steps:
1. Clone the repository to your local machine.
2. Build the compiler by running `make` in the root directory. This will iteratively run all of the makefiles within the subdirectories as well. The output will be a golf.exe (./golf).
3. Run the compiler on Go source code by executing:
   
     ```./golf inputfile```
  
4. The output will be MIPS assembly code printed to the console.

### 5. Usage Example
Here's an example of how to use the GoLF compiler:


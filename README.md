# Compiler Project

A standalone compiler implementation in C that performs lexical analysis, LL(1) parsing, AST construction, semantic analysis, and NASM code generation for a custom source language.

## Project Goals

- Build a complete front-end and basic back-end compiler pipeline in C.
- Validate source programs through lexical, syntax, and semantic checks.
- Generate executable NASM-compatible assembly for supported integer programs.
- Keep the implementation modular and readable across compiler stages.

## Features

- **Lexer**
  - Twin-buffer input stream handling.
  - Tokenization for identifiers, numbers, operators, delimiters, and keywords.
  - Lexical error reporting with line numbers.

- **Parser**
  - LL(1) grammar-driven predictive parsing.
  - Automated FIRST/FOLLOW set computation.
  - Parse table generation from grammar rules.
  - Parse tree construction.

- **AST Builder**
  - Parse-tree to AST transformation with inherited/synthesized structure handling.
  - Reduced tree representation for semantic and code generation stages.

- **Semantic Analysis**
  - Symbol table construction for variables, records, parameters, and functions.
  - Type checking for assignments, arithmetic expressions, boolean expressions, and returns.
  - Semantic error collection and reporting.

- **Code Generation**
  - NASM output for supported integer workflows.
  - Arithmetic and boolean expression code emission.
  - Basic control flow generation (`if-else`, `while`, I/O).

## Repository Layout

- `Compiler/driver.c` - Interactive compiler entry point and menu.
- `Compiler/lexer.*` - Tokenization and input buffer management.
- `Compiler/parser.*` - Grammar loading, FIRST/FOLLOW, parse table, parse tree.
- `Compiler/ast.*` - AST construction from parse tree.
- `Compiler/symbol_table.*` - Symbol table structures and lookup logic.
- `Compiler/type_checker.*` - Type compatibility and expression typing.
- `Compiler/semantic_analyzer.*` - Semantic validation passes.
- `Compiler/code_gen.*` - NASM code generation.
- `Compiler/testcases/` - Sample input programs.
- `Compiler/GRAMMAR.txt` - Grammar specification used by parser.

## Build Instructions

1. Go to the compiler directory:
   - `cd Compiler`
2. Build the executable:
   - `make`
3. The build generates:
   - `./compiler`

## Usage

Run the compiler with:

- `./compiler <input_source_file> <output_asm_file>`

Example:

- `./compiler testcases/main0.txt code.asm`

The CLI menu supports:

- Token stream printing
- Parse tree generation
- AST inspection and compression stats
- Symbol table and semantic checks
- NASM code generation (option `10`)

## Running Generated Assembly

If NASM is installed:

- `nasm -felf64 code.asm && gcc code.o && ./a.out`

## Notes

- The grammar and code generator currently target a constrained subset of language features for executable assembly output.
- Semantic checks are still performed for broader language constructs even when code generation support is limited.

## Author

- Abel Joise

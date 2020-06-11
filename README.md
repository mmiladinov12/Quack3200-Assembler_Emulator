# Quack3200 Assembeler and Emulator
This is an assembler and an emulator for a Quack3200 computer. This was a school project for cmps361 at Ramapo College of New Jersey.

## Quack3200 Assembly Code

### **Symbolic Op Codes**
- ADD 01 
  - Reg <-- c(Reg) + c(ADDR)
- SUB 02 
  - Reg <-- c(Reg) - c(ADDR)
- MULT 03 
  - Reg <-- c(Reg) * c(ADDR)
- DIV 04 
  - Reg <-- c(Reg) / c(ADDR)
- LOAD 05 
  - Reg <-- c(ADDR)
- STORE 06 
  - ADDR <-- c(Reg)
- READ 07 
  - A line is read and record the number found there in the specified address. The register value is ignored.
- WRITE 08 
  - c(ADDR) is displayed. The register value is ignored.
- B 09 
  - go to ADDR for next instruction. The register value is ignored.
- BM 10 
  - go to ADDR if c(Reg) < 0
- BZ 11 
  - go to ADDR if c(Reg) = 0
- BP 12 
  - go to ADDR if c(Reg) > 0
- HALT 13 
  - terminate execution. The register value is ignored.

### **Assembly Language Instructions**
- DC 
  - define constant. The constant is a decimal integer placed in the operand field.
- DS 
  - define storage. The operand specifies the number of words of storage to be set aside.
- ORG 
  - define origin. The operand specifies the address at which the translation of the next instruction will be generated,
- END
  - indicates that there are no additional statements to translate.

## Class Definitions

- stdafx.h - precompiled headers include file.
- stdafx.cpp - supports precompiled headers.
- Assem.cpp - the main program that launches the assembler.
- Assembler.h - definition of the assembler class.
- Assembler.cpp - implementation of the assembler class.
- FileAccess.h - definition of the class to perform file access.
- FileAccess.cpp - implementation of the class to perform file access.
- Instruction.h - the definition of the class to manipulate instructions. Includes error handling.
- SymTab.h - the definition of the class the manage the symbol table.
- SymTab.cpp - implementation of the class to manage the symbol table.
- Errors.h - the definition of the class to perform error reporting.
- Errors.cpp - the implementation of the class to perform error reporting.
- Emulator.h - the definition for the emulator class.

## Error Checks

1. Multiply defined labels.
2. Undefined label.
3. Syntax error in construction of the label or operands.
4. Illegal operation code.
5. Insufficient memory for the translation.
6. Missing end statement or the end statement is not the last one in the program.
7. Constant too large for Quack3200 memory.
8. Missing or extra operands

## Coding Standard

See *Coding_Standard.pdf* for all of the expected coding standards used in the project.

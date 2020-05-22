//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
Assembler::Assembler(int argc, char* argv[]) : m_facc(argc, argv) {}


/*
NAME

    Assembler::PassI - establishes the location of the labels

SYNOPSIS

    void Assembler::PassI();

DESCRIPTION

    This function will iterate through every line of the given
    assembly language program, establishing the locations of
    all labels, and storing them in a symbol table. 

*/
void Assembler::PassI() {
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    while (true) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        if (st == Instruction::ST_End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) {
            continue;
        }

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}


/*
NAME

    Assembler::PassII - generates instructions and error processing

SYNOPSIS

    void Assembler::PassII();

DESCRIPTION

    This function will reiterate the program's instructions, but with the addition
    of error checking and translating to machine code. The translations woll
    be stored in the Quack3600's main memory.

*/
void Assembler::PassII() {

    int loc = 0;        // Tracks the location of the instructions to be generated.
    string message;     // Stores the formatted error message

    // Rewinds the assembly program's source file
    m_facc.rewind();

    cout << "Translation of Program:" << endl << endl;
    cout << "Location " << setw(6) << "  Contents  " << setw(0) << " Original Statement" << endl << endl;

    // Successively process each line of source code.
    while (true) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // Records an error if there is no END statemtent
            message = "Program is missing an END statement";
            Errors::RecordError(message);
            break;
        }

        // Parses the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // Prints and skips the comment instructions
        if (st == Instruction::ST_Comment) {
            cout << setw(37) << line << endl;
            continue;
        }

        // Processes a series of formatting and validation error checks
        // Important: this must be done before handeling further instructions
        ErrorProccessing(message);

        // Print and skip error instructions
        if (st == Instruction::ST_Error) {
            cout << setw(35) << right << line << endl;
            continue;
        }

        // Prints and carries out the end instruction
        if (st == Instruction::ST_End) {

            cout << setw(31) << right << line << endl;

            // Checks to see if there is an instruction following
            // the END statement
            if (m_facc.GetNextLine(line)) {

                cout << setw(24) << right << line << endl;

                // Records an error if there was a line after the end statement
                message = "Program instructions does not stop after END statement";
                Errors::RecordError(message);
                break;
            }
            else {

                break;
            }
        }


        int content = 0;         // Contains the machine language translation
        string output = "";       // Printable machine language output

        // Prints the translation of machine language instructions
        if (st == Instruction::ST_MachineLanguage) {

            // Records an error if a register has an invalid format
            if (!m_inst.ValidateRegisterFormat()) {

                message = "Program has illegal Register";
                Errors::RecordError(message);
            }

            // Formatted translation of OpCode + register + address
            content = (((m_inst.GetOpCodeNum() * 10) + m_inst.GetRegisterNum()) * m_emul.MEMSZ) + m_symtab.LookupLocation(m_inst.GetOperand());

            // Adds a leading 0 if OpCode is a single digit
            if (to_string(content).length() < 8) {
                output = "0" + to_string(content);
            }
            else {
                output = to_string(content);
            }

            cout << "  " << right << loc << setw(14) << right << output << setw(3) << right << "   " << line << endl;

            // Builds up the memory of the emulator
            if (!m_emul.insertMemory(loc, content)) {

                message = "Insufficient memory for translation";
                Errors::RecordError(message);
            }
        }

        // Prints the translation of assembly language instructions
        if (st == Instruction::ST_AssemblerInstr) {

            // Determines if a constant is being printed or not
            if (m_inst.GetOpCode() == "dc") 
            {
                output = m_inst.GetOperand();

                // Adds leading 0's
                while (output.length() < 8) {
                    output = "0" + output;
                }

                cout << "  " << right << loc << setw(14) << right << output << setw(3) << right << "   " << line << endl;
            }
            else {
                cout << "  " << right << loc << setw(17) << right << "   " << line << endl;
            }
        }

        // Computes the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);

        // Determines if the next location is within the memory limit
        if (loc > m_emul.MEMSZ) {

            // Records out-of-bound errors
            message = "Program location out-of-bound";
            Errors::RecordError(message);
            break;
        }
    }

    // Formatted error display
    cout << endl << "__________________________________________________________" << endl;
    Errors::DisplayErrors();
    cout << "__________________________________________________________" << endl << endl << endl;

}

/*
NAME

    Assembler::ErrorProccessing - sequence of error checks

SYNOPSIS

    void Assembler::ErrorProccessing();
    a_message -> the error message to be recorded

DESCRIPTION

    This function is a helper function for PassII. It focuses on
    error checking the three main aspects of an assembly instruction line, 
    the label, OpCode, and Operand.

*/
void Assembler::ErrorProccessing(string& a_message) {

    // Records an error if a label has an invalid format
    if (!m_inst.ValidateLabelFormat()) {
        a_message = "Program has illegal label";
        Errors::RecordError(a_message);
    }

    // Verify that an existing label is in the symbol table
    if (m_inst.isLabel() == true && m_symtab.LookupSymbol(m_inst.GetLabel()) == false) {
        a_message = "Program does not contain the Label \"" + m_inst.GetLabel() + "\" in the symbol table";
        Errors::RecordError(a_message);
    }

    // Checks to see if a label is defined multiple times
    // in a program
    if (m_symtab.CheckMultiplyDefined(m_inst.GetLabel())) {
        a_message = "Program has multiply defined labels";
        Errors::RecordError(a_message);
    }

    // Reports error if the current OpCode is not
    // a legitimate OpCode
    if (!m_inst.OpCodeLookup()) {

        a_message = "Program uses an illegal OpCode";
        Errors::RecordError(a_message);

        // Overwrite illegal OpCode
        m_inst.SetOpCodeError();
    }

    // Records an error if an Operand has an invalid format
    if (!m_inst.ValidateOperandFormat()) {

        a_message = "Program has illegal Operand";
        Errors::RecordError(a_message);

        // Overwrite illegal operand
        m_inst.SetOperandError();
    }

    // Checks if an operand is either missing or if there
    // are too many operands
    if (m_inst.MissingOrExtraOperand()) {

        a_message = "Program has Extra or Missing Operand (This error will also occur if there is whitespace between the register and operand!!)";
        Errors::RecordError(a_message);
    }
}
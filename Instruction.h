//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() {};
    ~Instruction() {};

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage, 		// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End,                   	// end instruction.
        ST_Error                    // Default instruction
    };

    // Parse the Instruction.
    InstructionType ParseInstruction(string a_buff);

    InstructionType DetermineInstructionType();

    // Computes the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // Converts a string to a numeric value
    int ConvertToNumeric(string a_value) {
        return stoi(a_value, NULL);
    }

    // Determines if OpCode is legal
    bool OpCodeLookup() {
        return isAssembly(m_OpCode) || m_MachineOpCodes[m_OpCode];
    }

    bool ValidateLabelFormat();

    bool ValidateOperandFormat();

    bool ValidateRegisterFormat();

    // Checks for missing or extra operands
    bool MissingOrExtraOperand();

    // Determines if a label is blank.
    bool isLabel() { return !m_Label.empty(); }

    // Regular expression used to determine if
    // a string with any sign is a number
    bool isNumber(string a_value) {
        return regex_match(a_value, regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
    }

    // Checks if an OpCode is an assembly OpCode
    bool isAssembly(string a_OpCode);

    // Error Overwrite functions
    void SetOpCodeError() { m_OpCode = "??"; }

    void SetOperandError() { m_Operand = "?????"; }

    // Sets most member variables to their default values
    void SetDefault();

    // Getter Functions
    string& GetLabel() { return m_Label; }
    string& GetOperand() { return m_Operand; }
    string& GetOpCode() { return m_OpCode; }
    int GetOpCodeNum() { return m_NumOpCode; }
    int GetRegisterNum() { return m_NumRegister; }

private:

    void SetLabelOpcodeEtc(const string& a_buff);


    // The elemements of a instruction
    string m_Label = "";         // The label.
    string m_OpCode = "";        // The symbolic op code.
    string m_Register = "";      // The register value.
    string m_Operand = "";       // The operand.


    string m_instruction = "";   // The original instruction.

    // Derived values.
    int m_NumOpCode = -1;        // The numerical value of the op code.
    int m_NumRegister = -1;	    // the numberic value for the register.
    InstructionType m_type = ST_Error;    // The type of instruction.

    bool m_ExtraOperand = false;       // == true if there is an extra operand
    bool m_IsNumericOperand = false;    // == true if the operand is numeric.
    int m_OperandValue = -1;     // The value of the operand if it is numeric.

    // The map containing all machine language op codes and their numeric equivalent
    map<string, int> m_MachineOpCodes = { {"add",1}, {"sub",2}, {"mult",3}, {"div",4}, {"load",5}, {"store", 6}, {"read", 7}, {"write", 8}, {"b", 9}, {"bm", 10}, {"bz", 11}, {"bp", 12}, {"halt", 13} };
    
    // The vector containing all assembly language op codes
    vector<string> m_AssemblyOpCodes = { "dc", "ds", "org", "end" };

};

//
//		Implementation of the Instruction class.
//
#include "stdafx.h"
#include <sstream>
#include "Instruction.h"

/*
NAME

	Instruction::ParseInstruction - Break down instructions and determine data type

SYNOPSIS

	Instruction::InstructionType Instruction::ParseInstruction(string a_buff);
	a_buff -> The buffer carrying the original instruction

DESCRIPTION

	This function resets the member data of previous instructions and saves
	the original version of the current instruction. It then parses out any
	comments, and calls functions that seperates the elementary data
	and establish the instruction data type.

RETURNS

	The determined data type of the instruction 

*/
Instruction::InstructionType Instruction::ParseInstruction(string a_buff) {
	// Set member variables to default
	SetDefault();

	// Make copy of original instruction
	m_instruction = a_buff;

	// Get rid of everything after the semicolon if it exists.  "aaa;"
	size_t isemi = a_buff.find(';');
	if (isemi != string::npos)
	{
		a_buff = a_buff.substr(0, isemi);
	}

	// Parse instruction into basic elements.
	SetLabelOpcodeEtc(a_buff);

	m_type = DetermineInstructionType();
	
	// Records the numeric machine language OpCode
	if (m_type == InstructionType::ST_MachineLanguage) {
		m_NumOpCode = m_MachineOpCodes[m_OpCode];
	}

	return m_type;
}

/*
NAME

	Instruction::DetermineInstructionType - establishes instruction data type

SYNOPSIS

	Instruction::InstructionType Instruction::DetermineInstructionType();

DESCRIPTION

	This function uses the OpCode and label to specify the type of
	instruction currently being evaluated.

RETURNS

	An instruction data type

*/
Instruction::InstructionType Instruction::DetermineInstructionType() {
	
	if (m_MachineOpCodes[m_OpCode]) {
		return InstructionType::ST_MachineLanguage;
	}
	else if (m_OpCode == "dc" || m_OpCode == "ds" || m_OpCode == "org") {
		return InstructionType::ST_AssemblerInstr;
	}
	else if (m_OpCode == "end") {
		return InstructionType::ST_End;
	}
	else if (m_OpCode == "" && m_Label == "") {
		return InstructionType::ST_Comment;
	}
	else {
		return InstructionType::ST_Error;
	}
}

/*
NAME

	Instruction::LocationNextInstruction - establishes next location

SYNOPSIS

	int Instruction::LocationNextInstruction(int a_loc);
	a_loc -> the current location

DESCRIPTION

	This function will increment the current location for all types
	of OpCodes besides "ds" and "org" codes which provide a specified
	location.

RETURNS
	
	The location of the next instruction

*/
int Instruction::LocationNextInstruction(int a_loc) {

	if (m_OpCode == "org" || m_OpCode == "ds" && m_IsNumericOperand) {
		return a_loc + ConvertToNumeric(m_Operand);
	}

	return a_loc + 1;
}

/*
NAME

	Instruction::ValidateLabelFormat - upholds the label format specifications

SYNOPSIS

	bool Instruction::ValidateLabelFormat();

DESCRIPTION

	This function first determines if there is a label in the instruction
	set. If so, it checks if the label is 1-10 characters in length and starts
	with an alphabetical letter.

RETURNS

	Whether the format is valid or not

*/
bool Instruction::ValidateLabelFormat() {

	if (!isLabel()) {
		return true;
	}

	return m_Label.length() >= 1 && m_Label.length() <= 10 && isalpha(m_Label[0]) > 0;

}

/*
NAME

	Instruction::ValidateOperandFormat - upholds the operand format specifications

SYNOPSIS

	bool Instruction::ValidateOperandFormat();

DESCRIPTION

	This function first determines if an operand exists in the
	instruction set. If so, it then establishes whether the OpCode 
	is an assembly instruction, machine code instruction, or neither. 
	Assembly instructions are then tested to see if they conform to
	the specified numeric range and Machine code instructions are tested
	for symbolic specifications.

RETURNS

	Whether the format is valid or not

*/
bool Instruction::ValidateOperandFormat() {

	if (m_Operand == "") {
		return true;
	}

	if (isAssembly(m_OpCode)) {

		// Assembly OpCodes can only have numeric operands
		if (m_IsNumericOperand) {
			return m_OperandValue >= -99999 && m_OperandValue <= 99999;
		}
		else {
			return false;
		}

	}
	else if (m_MachineOpCodes[m_OpCode]) {

		// Machine language OpCodes can only have symbolic operands
		if (!m_IsNumericOperand) {
			return m_Operand.length() >= 1 && m_Operand.length() <= 10 && isalpha(m_Operand[0]) > 0;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

/*
NAME

	Instruction::ValidateRegisterFormat - upholds the register format specifications

SYNOPSIS

	bool Instruction::ValidateRegisterFormat();

DESCRIPTION

	This function first determines if a register exists in the
	instruction set. Then it checks if the registers are in
	the 0-9 numeric range.
	
RETURNS

	Whether the format is valid or not

*/
bool Instruction::ValidateRegisterFormat() {
	if (m_Register == "") {
		return true;
	}
	
	return (m_NumRegister >= 0 && m_NumRegister <= 9);
}

/*
NAME

	Instruction::MissingOrExtraOperand - identifies excess or missing operands

SYNOPSIS

	bool Instruction::MissingOrExtraOperand();

DESCRIPTION

	This function factors in the possibilities of any extra characters
	or values that appear after the originially declared operand. It also
	examines OpCodes that do not require operands, such as "halt" and "end",
	and checks if they have an operand.

RETURNS

	Whether missing or excess operands were detected

*/
bool Instruction::MissingOrExtraOperand() {

	// Checks if extra values were recorded in instruction parsing
	if (m_ExtraOperand) {
		return true;
	}

	// Checks if standalone OpCodes have an operand
	if (m_OpCode == "halt" || m_OpCode == "end" && m_Operand == "") {
		return false;
	}

	// Examines whether assembly or machine codes are missing operands
	if (isAssembly(m_OpCode) && m_Operand == "") {
		return true;
	}
	else if (m_MachineOpCodes[m_OpCode] && m_Operand == "") {
		return true;
	}
	else {
		return false;
	}
}

/*
NAME

	Instruction::isAssembly - Determines if an OpCode is an assembly instruction

SYNOPSIS

	bool Instruction::isAssembly(string a_OpCode);
	a_OpCode -> OpCode being reviewed

DESCRIPTION

	This function loops through the list of assembly OpCodes and 
	checks if the given OpCode matches any of them.

RETURNS

	Whether the operation code is an assembly instruction

*/
bool Instruction::isAssembly(string a_OpCode) {

	for (string OpCode : m_AssemblyOpCodes) {
		if (OpCode == a_OpCode) {
			return true;
		}
	}

	return false;
}

/*
NAME

	Instruction::SetDefault - sets member variables to default

SYNOPSIS

	void Instruction::SetDefault();

DESCRIPTION

	This function takes a majority of the member variables and sets them 
	to a default value.

*/
void Instruction::SetDefault() {

	m_instruction = "";
	m_NumOpCode = -1;
	m_NumRegister = -1;
	m_type = ST_Error;
	m_ExtraOperand = false;
	m_IsNumericOperand = false;
	m_OperandValue = -1;
}

/*
NAME

	Instruction::SetLabelOpcodeEtc - sets the values of parsed instruction variables

SYNOPSIS

	void Instruction::SetLabelOpcodeEtc(const string& a_buff);
	a_buff -> the instruction buffer that contains no comments

DESCRIPTION

	This function uses istringstreams to parse the label, OpCode, operand, 
	and register from an original assembly instruction. It establishes
	some derived values such as numeric and booleans values. It also
	gurantees that the OpCode fits its lowercase formatting
	requirement.

*/
void Instruction::SetLabelOpcodeEtc(const string& a_buff)
{
	m_Label = m_OpCode = m_Register = m_Operand = "";

	istringstream ins(a_buff);

	string a1, a2, a3, a4;
	ins >> a1 >> a2 >> a3 >> a4;

	// If there is no data, this line must have been empty or just had a comment.
	if (a1.empty())
	{
		return;
	}
	// If the first character is not whitespace, it is assumes to be a label.
	if (a_buff[0] != ' ' && a_buff[0] != '\t')
	{
		m_Label = a1;
		m_OpCode = a2;
		m_Operand = a3;

		// A fourth value represents an extra operant value when there is 
		// a label
		if (a4 != "") {
			m_ExtraOperand = true;
		}
	}
	else
	{
		// A third value indicates an extra operant when there is no label
		m_OpCode = a1;
		m_Operand = a2;
		if (a3 != "") {
			m_ExtraOperand = true;
		}
	}
	// Check operand for comma and parse accordingly 
	size_t icomma = m_Operand.find(',');
	if (icomma != string::npos)
	{
		m_Register = m_Operand.substr(0, icomma);
		m_Operand = m_Operand.substr(icomma + 1);

		m_NumRegister = ConvertToNumeric(m_Register);
	}
	else
	{
		m_Register = "9";

		m_NumRegister = ConvertToNumeric(m_Register);
	}

	// Convert string values to numeric values
	if (isNumber(m_Operand)) {
		m_IsNumericOperand = true;
		m_OperandValue = ConvertToNumeric(m_Operand);
	}

	// Sets OpCodes to lower case
	transform(m_OpCode.begin(), m_OpCode.end(), m_OpCode.begin(), ::tolower);


}
//
//		Implementation of the Errors class.
//
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;
bool Errors::m_WasErrorMessages = false;

/*
NAME

    Errors::DisplayErrors - prints list of errors

SYNOPSIS

    void Errors::DisplayErrors();

DESCRIPTION

    This function takes all of the errors gathered from iterating 
    through the instruction set and lists them out.

*/
void Errors::DisplayErrors() {
    // Displays all errors
    for (string error : m_ErrorMsgs) {
        cout << error << endl;
    }

    m_ErrorMsgs.clear();
}
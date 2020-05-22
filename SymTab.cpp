//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table

SYNOPSIS

    void AddSymbol( char *a_symbol, int a_loc );
    a_symbol -> the new symbol to be added
    s_loc -> the address of where the symbol will be added

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.

*/
void SymbolTable::AddSymbol(string& a_symbol, int a_loc)
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) 
    {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME

    SymbolTable::DisplaySymbolTable - presents the symbol table

SYNOPSIS

    void SymbolTable::DisplaySymbolTable();

DESCRIPTION

    This function will output all symbols in a formatted table.

*/
void SymbolTable::DisplaySymbolTable() 
{

    int symbolCount = 0;

    cout << "Symbol Table:" << endl << endl;
    cout << "Symbol # " << setw(6) << " Symbol " << setw(0) << " Location" << endl;
    
    // Displays the amount of symbols, symbol name, and location
    for (map<string, int>::iterator it = m_symbolTable.begin(); it != m_symbolTable.end(); it++) 
    {

        cout << setw(2) << right << symbolCount << "         " << setw(6) << left << it->first << "  " << right << it->second << endl;
        symbolCount++;
    }

    cout << "__________________________________________________________" << endl << endl << endl;
}
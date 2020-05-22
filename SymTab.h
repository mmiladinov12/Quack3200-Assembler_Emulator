//
//		Symbol table class.
//
#pragma once



// This class is our symbol table.
class SymbolTable {

public:
    SymbolTable() {};
    ~SymbolTable() {};

    const int multiplyDefinedSymbol = -999;

    // Adds a new symbol to the symbol table.
    void AddSymbol(string& a_symbol, int a_loc);

    // Displays the symbol table.
    void DisplaySymbolTable();

    // Returns the location of a specified symbol
    int LookupLocation(string& a_symbol) { return m_symbolTable[a_symbol]; }

    // Check if a symbol exists in the symbol table.
    bool LookupSymbol(string& a_symbol) {
        return m_symbolTable.find(a_symbol) != m_symbolTable.end();
    }

    // Determines if a symbol was added to the table multiple times
    bool CheckMultiplyDefined(const string& a_symbol) {
        return m_symbolTable[a_symbol] == multiplyDefinedSymbol;
    }

private:

    // This is the actual symbol table.  The symbol is the key to the map.
    map<string, int> m_symbolTable;

};
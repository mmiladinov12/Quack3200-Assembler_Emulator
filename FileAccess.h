#pragma once

#include <fstream>
#include <stdlib.h>
#include <string>

class FileAccess {

public:

    // Opens the file.
    FileAccess(int argc, char* argv[]);

    // Closes the file.
    ~FileAccess();

    // Gets the next line from the source file.
    bool GetNextLine(string& a_buff);

    // Puts the file pointer back to the beginning of the file.
    void rewind();

private:

    ifstream m_sfile;		// Source file object.
};
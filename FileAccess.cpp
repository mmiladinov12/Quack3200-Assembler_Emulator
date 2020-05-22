//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"
#include <iostream>

/*
NAME

    FileAccess::FileAccess - opens an assembly program file

SYNOPSIS

    FileAccess::FileAccess(int argc, char* argv[]);
    int argc -> the amount of command line arguments given
    char* argv[] -> a list of provided file names

DESCRIPTION

    This constructor takes in a command line argument abd opens the
    file to an assembly program. It also provides reliability with
    file error checking.

*/
FileAccess::FileAccess(int argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) 
    {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }

    m_sfile.open(argv[1], ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) 
    {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit(1);
    }
}

/*
NAME

    FileAccess::~FileAccess - closes an opened file

SYNOPSIS

    FileAccess::~FileAccess();

DESCRIPTION

    This destructor closes the file of the assembly program once the
    program has been completed and terminated.

*/
FileAccess::~FileAccess()
{

    m_sfile.close();
}

/*
NAME

    FileAccess::GetNextLine - Get the next line from the file.

SYNOPSIS

    bool FileAccess::GetNextLine(string& a_buff);
    a_buff -> The storage buffer for the next instruction

DESCRIPTION

    This function checks to see if the end of the file has been
    reached. If not, the next instruction line is given to the buffer
    and the function returns successfully.

RETURNS

    Whether or not the next line was aquirable

*/
bool FileAccess::GetNextLine(string& a_buff)
{
    // If there is no more data, return false.
    if (m_sfile.eof()) 
    {

        return false;
    }
    getline(m_sfile, a_buff);

    // Return indicating success.
    return true;
}

/*
NAME

    FileAccess::rewind - restart the program file

SYNOPSIS

    void FileAccess::rewind();

DESCRIPTION

    This function reinstates the original condition of the file
    including file flags and starting point.

*/
void FileAccess::rewind()
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg(0, ios::beg);
}
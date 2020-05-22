#pragma once

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Errors {

public:

    // Records an error message.
    static void RecordError(const string a_emsg) {
        m_ErrorMsgs.push_back(a_emsg);
        m_WasErrorMessages = true;
    }

    // Displays the collected error message.
    static void DisplayErrors();

private:

    static vector<string> m_ErrorMsgs;
    static bool m_WasErrorMessages;
};
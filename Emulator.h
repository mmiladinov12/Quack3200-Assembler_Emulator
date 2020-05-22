#pragma once

class emulator {

public:

	const static int MEMSZ = 100000;	// The size of the memory of the Quack3200.
	emulator() {

		memset(m_memory, 0, MEMSZ * sizeof(int));
		memset(m_reg, 0, 10 * sizeof(int));
	}

	// Records instructions and data into Quack3200 memory.
	bool insertMemory(int a_location, int a_contents) {
		if (a_location >= 0 && a_location < MEMSZ) {
			m_memory[a_location] = a_contents;
			return true;
		}
		else {
			cerr << "Grumble gumble" << endl;
			return false;
		}
	}

	// Runs the Quack3200 program recorded in memory.
	bool runProgram() {
		int loc = 100;

		cout << "Results from emulating program :" << endl << endl;
		while (true) {
			int contents = m_memory[loc];  // 01455555
			int opcode = contents / 1000000;
			int reg = contents / 100000 % 10; // 14
			int address = contents % 100000;

			switch (opcode) {
			// ADD instruction
			case 1:
				m_reg[reg] += m_memory[address];
				loc += 1;
				continue;
			// SUB instruction
			case 2:
				m_reg[reg] -= m_memory[address];
				loc += 1;
				continue;
			// MULT instruction
			case 3:
				m_reg[reg] *= m_memory[address];
				loc += 1;
				continue;
			// DIV instruction
			case 4:
				m_reg[reg] /= m_memory[address];
				loc += 1;
				continue;
			// LOAD instruction
			case 5:
				m_reg[reg] = m_memory[address];
				loc += 1;
				continue;
			// STORE instruction
			case 6:
				m_memory[address] = m_reg[reg];
				loc += 1;
				continue;
			// READ instruction
			case 7:
				int input;
				cout << "? ";
				cin >> input;
				m_memory[address] = input;
				loc += 1;
				continue;
			// WRITE instruction
			case 8:
				cout << m_memory[address] << endl;
				loc += 1;
				continue;
			// Branch instruction
			case 9:
				loc = address;
				continue;
			// Branch Minus instruction
			case 10:
				if (m_reg[reg] < 0) {
					loc = address;
				}
				else {
					loc += 1;
				}
				continue;
			// Branch Zero instruction
			case 11:
				if (m_reg[reg] == 0) {
					loc = address;
				}
				else {
					loc += 1;
				}
				continue;
			// Branch Positive instruction
			case 12:
				if (m_reg[reg] > 0) {
					loc = address;
				}
				else {
					loc += 1;
				}
				continue;
			// HALT instruction
			case 13:
				cout << endl << "End of emulation" << endl;
				exit(0);
			default:
				cerr << "Illegal opcode" << endl;
				exit(1);
			}
		}
	}

private:

	int m_memory[MEMSZ];    // The memory of the Quack3200.
	int m_reg[10];		    // The accumulator for the Quack3200
};
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

void shiftRegister(long long int bx, long long int gx)
{
    // Create vector to represent bx as a register (data word)
    vector<bool> bxRegister;
    while(bx>0)
    {
        bxRegister.push_back(bx%10);
        bx/=10;
    }

    // Reverse bx vector for proper polynomial representation and since we want to pop off msb when dividing
    reverse(bxRegister.begin(), bxRegister.end());

    // Create vector to represent gx polynomial (crc feedback polynomial XOR gates)
    vector<bool> gxRegister;
    while(gx>0)
    {
        gxRegister.push_back(gx%10);
        gx/=10;
    }

    // Create shift registers initialized to zero
    vector<bool> cshiftRegs;
    for (int i = 0; i < gxRegister.size() - 1; i++)
    {
        cshiftRegs.push_back(0);
    }



    // Show initial state
    cout << "Shift Registers: " << cshiftRegs.size() << endl;
    cout << "XOR Gates at inputs for shift registers: ";
    for (int i = 0; i < gxRegister.size() - 1; i++)
    {
        if (gxRegister[i])
        {
            cout << "C" << i << " ";
        }
    }

    cout << endl << endl;
    // Print out clock
    int clockCount = 0;
    cout << "Clock: " << clockCount << endl;
    // Print out  C Shift registers
    cout << "C Shift Register Contents: [" << to_string(cshiftRegs.size()-1) << "-0]" << endl;
    for (int i = 0; i < cshiftRegs.size(); i++)
    {
        cout << cshiftRegs[i] << " ";
    }
    cout << endl;
    // Print out B(x) register
    cout << "B(x) register contents: " << endl;
    for (int i = 0; i < bxRegister.size(); i++)
    {
        cout << bxRegister[i] << " ";
    }
    cout << endl;
    // Print out inputs and xor gates
    cout << "Input: " << "-" << endl;
    for (int i = 0; i < gxRegister.size() - 1; i++)
    {
        if (gxRegister[i])
        {
            cout << "XOR C" << i << ": " << "-" << endl;
        }
    }
    cout << endl;

    // Setup CSV file
    ofstream myFile("hoangCRC-table-output.csv");

    // Create column names
    myFile << "Clock,";
    myFile << "Input,";

    for (int i = cshiftRegs.size()-1; i >= 0; i--)
    {
        myFile << "Reg " + to_string(i) + ",";
    }

    for (int i = 0; i < gxRegister.size() - 1; i++)
    {
        if (gxRegister[i])
        {
            // Add cols to csv file for xor gates
            myFile << "XOR C" + to_string(i) + ",";
        }
    }

    // Next row
    myFile << "\n";

    // Output initial values to csv
    myFile << to_string(clockCount) + ",";
    myFile << "-,";
    for (int i = 0; i < cshiftRegs.size(); i++)
    {
        myFile << to_string(cshiftRegs[i]) + ",";
    }
    for (int i = 0; i < gxRegister.size() - 1; i++)
    {
        if (gxRegister[i])
        {
            // Add cols to csv file for xor gates
            myFile << "-,";
        }
    }
    // Move to next row in csv
    myFile << "\n";





    // Perform modulo 2 division
    while (!bxRegister.empty())
    {
        vector<bool>oldcshiftRegs = cshiftRegs;

        for (int i = 0; i < cshiftRegs.size(); i++)
        {
            // If gx polynomial has 1 at bit position,
            // perform xor operation (there is an xor gate
            // connected to feedback which is the last cshift
            // register value)
            if (gxRegister[i] == 1)
            {
                // If not first shift register, take in value from xor of previous shift register and feedback
                if (i != 0)
                {
                    cshiftRegs[i] = oldcshiftRegs[i-1] != oldcshiftRegs[oldcshiftRegs.size()-1];
                }
                // If first shift register C0, take in value from top bit value from bx register and feedback
                else
                {
                    cshiftRegs[i] = bxRegister.front() != oldcshiftRegs[oldcshiftRegs.size()-1];
                }
            }
            // No xor gate at input for shift gate
            else
            {
                if (i != 0)
                {
                    cshiftRegs[i] = oldcshiftRegs[i-1];
                }
                else
                {
                    cshiftRegs[i] = bxRegister.front();
                }
            }
        }

        bool bxinput = bxRegister.front();
        // Shift out top bit of data word
        bxRegister.erase(bxRegister.begin());

        // Print out clock
        clockCount++;
        myFile << to_string(clockCount) + ",";
        myFile << to_string(bxinput) + ",";
        cout << "Clock: " << clockCount << endl;
        cout << "C Shift Register Contents: [" << to_string(cshiftRegs.size()-1) << "-0]" << endl;
        for (int i = cshiftRegs.size()-1; i >= 0; i--)
        {
            cout << cshiftRegs[i] << " ";
            myFile << to_string(cshiftRegs[i]) + ",";
        }
        cout << endl;
        cout << "B(x) register contents: " << endl;
        for (int i = 0; i < bxRegister.size(); i++)
        {
            cout << bxRegister[i] << " ";
        }
        cout << endl;
        cout << "Input: " << bxinput << endl;
        for (int i = 0; i < gxRegister.size() - 1; i++)
        {
            if (gxRegister[i])
            {
                cout << "XOR Output to C" << i << ": " << cshiftRegs[i] << endl;
                myFile << to_string(cshiftRegs[i]) + ",";
            }
        }
        cout << endl;
        myFile << "\n";
    }


    // Print out division result
    myFile << "\n\n";
    myFile << "Remainder:,";
    cout << "Modulo 2 Division CRC Check Result of B(x) by g(x) (Final C Shift Register Contents): " << endl;
    for (int i = cshiftRegs.size()-1; i >= 0; i--)
    {
        cout << cshiftRegs[i] << " ";
        myFile << to_string(cshiftRegs[i]) + ",";
    }

    // Close fstream file
    myFile.close();
}

int main() {
    // Ask user to B(x) polynomial in binary
    cout << "Input an n+(n-k) binary data value representation for B(x): ";
    long long int bx;
    cin >> bx;

    // Ask user to G(x) polynomial in binary
    cout << "Input an n-k+1 binary data value representation for g(x): ";
    long long int gx;
    cin >> gx;

    shiftRegister(bx, gx);


    return 0;
}

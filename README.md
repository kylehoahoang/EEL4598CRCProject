# EEL4598CRCProject
C++ program to emulate CRC receiver shift registers.

Program will ask user to input two polynomials in binary form:
B(x) - CRC dividend and codeword to be checked
g(x) - CRC code to divide B(x).

How to Run:
Compile and run main.cpp in an C++ IDE such as CLion or VSCode and interact through console.

Example Input:
Input an n+(n-k) binary data value representation for B(x): 1100000
Input an n-k+1 binary data value representation for g(x): 1011

Output: 
Console logged register values of B(x) register, shift registers, and XOR gates on every clock.
Remainder result of modulo 2 division (CRC check)
CSV file named hoangCRC-table-output.csv in the same directory as the executable with table of same information as console.

More Info:
If remainder is all zero's, CRC success and information was received correctly.
Else, if there is a remainder, the information was not received correctly.

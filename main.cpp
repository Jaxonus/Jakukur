#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;
string 
chi1="01010101010101010101010101010101010101010",
chi2="0011001100110011001100110011001", 
chi3="00011100011100011100011100011",
chi4="00001111000011110000111100",
chi5="00000111110000011111000",
mu1="0101010101010101010101010101010101010101010101010101010101010",
mu2, 
psi1, 
psi2, 
psi3, 
psi4, 
psi5, 
input, output;
int ic1, ic2, ic3, ic4, ic5, im1, im2, ip1, ip2, ip3, ip4, ip5,iin,iou;
map<char, int>Baudot{
	{' ',0b000100},{'q',0b011101},{'w',0b011001},{'e',0b010000},{'r',0b001010},
	{'t',0b000001},{'y',0b010101},{'u',0b011100},{'i',0b001100},{'o',0b000011},
	{'p',0b001101},{'a',0b011000},{'s',0b010100},{'d',0b010010},{'f',0b010110},
	{'g',0b001011},{'h',0b000101},{'j',0b011010},{'k',0b011110},{'l',0b001001},
	{'z',0b010001},{'x',0b010111},{'c',0b001110},{'v',0b001111},{'b',0b010011},
	{'n',0b000110},{'m',0b000111},
	{'1',0b111101},{'2',0b111001},{'3',0b110000},{'4',0b101010},{'5',0b100001},
	{'6',0b110101},{'7',0b111100},{'8',0b101100},{'9',0b100011},{'0',0b101101},
	{'-',0b111000},{'$',0b110010},{'!',0b110110},{'&',0b101011},{'#',0b100101},
	{39 ,0b111010},{'(',0b111110},{')',0b101001},{'+',0b110001},{'/',0b110111},
	{':',0b001110},{'=',0b001111},{'?',0b010011},{',',0b000110},{'.',0b000111}
};
int main()
{

}
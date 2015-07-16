#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include "Specification.h"
using namespace std;
#define DEBUG2

// hexadecimal to unsigned decimal number
int toUnint(string str) {
	int length = (int) str.length();
	int sum = 0;

	int tmp;
	for (int i = 0; i < length; i++) {
		tmp = htd(str.at(i));
		sum += (int) (pow(16.0, length - i - 1)) * tmp;
	}

	return sum;
}

int toInt(string str) {
	int msb = toBincode(str[0]).at(0) - '0';
	int value;

	// 2's complement
	string com2s = "";

	if (msb) {
		for (unsigned int i = 0; i < str.length() - 1; i++)
			com2s += dth(15 - htd(str[i]));
		com2s += dth(16 - htd(str[str.length() - 1]));
		value = toUnint(com2s);
	} else
		value = toUnint(str);
	if (msb)
		value *= -1;

	return value;
}

string toHex(int n, int digit) {
	string restr = "";
	char b[8];
	sprintf(b, "%08X", n);
	for (int i = 8 - digit; i < 8; i++)
		restr += b[i];
	return restr;
}

// unsigned decimal number to binary
string toBincode(int n) {
	switch (n) {
		case 0:
			return "0000";
		case 1:
			return "0001";
		case 2:
			return "0010";
		case 3:
			return "0011";
		case 4:
			return "0100";
		case 5:
			return "0101";
		case 6:
			return "0110";
		case 7:
			return "0111";
		case 8:
			return "1000";
		case 9:
			return "1001";
		case 10:
			return "1010";
		case 11:
			return "1011";
		case 12:
			return "1100";
		case 13:
			return "1101";
		case 14:
			return "1110";
		case 15:
			return "1111";
		default:
			return "ERROR";
	}
}

// hex to binary
string toBincode(char ch) {
	switch (ch) {
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
		case 'a':
			return "1010";
		case 'B':
		case 'b':
			return "1011";
		case 'C':
		case 'c':
			return "1100";
		case 'D':
		case 'd':
			return "1101";
		case 'E':
		case 'e':
			return "1110";
		case 'F':
		case 'f':
			return "1111";
		default:
			return "ERROR";
	}
}

//Hexadecimal to decimal
int htd(char ch) {
	switch (ch) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
		case 'a':
			return 10;
		case 'B':
		case 'b':
			return 11;
		case 'C':
		case 'c':
			return 12;
		case 'D':
		case 'd':
			return 13;
		case 'E':
		case 'e':
			return 14;
		case 'F':
		case 'f':
			return 15;
		default:
			return -1;
	}
}

//Decimal to hexadecimal
char dth(int num) {
	switch (num) {
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';
		default:
			return 'G';
	}
}

// look up the op table
//if type is true use str lookup, else use num
//return string of operation code and format used point
string LookUpTable(string str, int num, bool type, int offset, int *format, int *opNum) {
	int tmp;
	string out;

	if (type)
		tmp = toUnint(str);
	else
		tmp = num;

	tmp -= offset;

	switch (tmp) {
		case 192:
			*format = 1;
			out = "FLOAT";
			break;
		case 196:
			*format = 1;
			out = "FIX";
			break;
		case 200:
			*format = 1;
			out = "NORM";
			break;
		case 240:
			*format = 1;
			out = "SIO";
			break;
		case 244:
			*format = 1;
			out = "HIO";
			break;
		case 248:
			*format = 1;
			out = "TIO";
			break;
		case 144:
			*format = 2;
			out = "ADDR";
			break;
		case 148:
			*format = 2;
			out = "SUBR";
			break;
		case 152:
			*format = 2;
			out = "MULR";
			break;
		case 156:
			*format = 2;
			out = "DIVR";
			break;
		case 160:
			*format = 2;
			out = "COMPR";
			break;
		case 164:
			*format = 2;
			out = "SHIFTL";
			break;
		case 168:
			*format = 2;
			out = "SHIFTR";
			break;
		case 172:
			*format = 2;
			out = "RMO";
			break;
		case 176:
			*format = 2;
			out = "SVC";
			break;
		case 180:
			*format = 2;
			out = "CLEAR";
			break;
		case 184:
			*format = 2;
			out = "TIXR";
			break;
		case 0:
			*format = 3;
			out = "LDA";
			break;
		case 4:
			*format = 3;
			out = "LDX";
			break;
		case 8:
			*format = 3;
			out = "LDL";
			break;
		case 12:
			*format = 3;
			out = "STA";
			break;
		case 16:
			*format = 3;
			out = "STX";
			break;
		case 20:
			*format = 3;
			out = "STL";
			break;
		case 24:
			*format = 3;
			out = "ADD";
			break;
		case 28:
			*format = 3;
			out = "SUB";
			break;
		case 32:
			*format = 3;
			out = "MUL";
			break;
		case 36:
			*format = 3;
			out = "DIV";
			break;
		case 40:
			*format = 3;
			out = "COMP";
			break;
		case 44:
			*format = 3;
			out = "TIX";
			break;
		case 48:
			*format = 3;
			out = "JEQ";
			break;
		case 52:
			*format = 3;
			out = "JGT";
			break;
		case 56:
			*format = 3;
			out = "JLT";
			break;
		case 60:
			*format = 3;
			out = "J";
			break;
		case 64:
			*format = 3;
			out = "AND";
			break;
		case 68:
			*format = 3;
			out = "OR";
			break;
		case 72:
			*format = 3;
			out = "JSUB";
			break;
		case 76:
			*format = 3;
			out = "RSUB";
			break;
		case 80:
			*format = 3;
			out = "LDCH";
			break;
		case 84:
			*format = 3;
			out = "STCH";
			break;
		case 88:
			*format = 3;
			out = "ADDF";
			break;
		case 92:
			*format = 3;
			out = "SUBF";
			break;
		case 96:
			*format = 3;
			out = "MULF";
			break;
		case 100:
			*format = 3;
			out = "DIVF";
			break;
		case 104:
			*format = 3;
			out = "LDB";
			break;
		case 108:
			*format = 3;
			out = "LDS";
			break;
		case 112:
			*format = 3;
			out = "LDF";
			break;
		case 116:
			*format = 3;
			out = "LDT";
			break;
		case 120:
			*format = 3;
			out = "STB";
			break;
		case 124:
			*format = 3;
			out = "STS";
			break;
		case 128:
			*format = 3;
			out = "STF";
			break;
		case 132:
			*format = 3;
			out = "STT";
			break;
		case 136:
			*format = 3;
			out = "COMPF";
			break;
		case 208:
			*format = 3;
			out = "LPS";
			break;
		case 212:
			*format = 3;
			out = "STI";
			break;
		case 216:
			*format = 3;
			out = "RD";
			break;
		case 220:
			*format = 3;
			out = "WD";
			break;
		case 224:
			*format = 3;
			out = "TD";
			break;
		case 232:
			*format = 3;
			out = "STSW";
			break;
		case 236:
			*format = 3;
			out = "SSK";
			break;
		default:
			out = "ERROR";
			break;
	}

	if (!strcmp(out.c_str(), "ERROR")) {
		// imply the str is data not op code
		*format = 0;
		*opNum = 0;
		return out;
	} else {
		*opNum = tmp;
		return out;
	}
}

#include "Registers.h"
#include <string>
using std::string;

Registers::Registers() {
	// TODO Auto-generated constructor stub
	A = "000000";
	X = "000000";
	L = "FFFFFF";
	B = "000000";
	S = "000000";
	T = "000000";
	F = "000000";
	SW = "000000";
	PC = "000000";
}

Registers::~Registers() {
	// TODO Auto-generated destructor stub
}

//using number to access register content
//A = 0, X = 1, L = 2, B = 3, S = 4, T = 5, F = 6, PC = 8, SW = 9
string Registers::Store(int n) const {
	switch (n) {
		case 0:
			return this->A;
		case 1:
			return this->X;
		case 2:
			return this->L;
		case 3:
			return this->B;
		case 4:
			return this->S;
		case 5:
			return this->T;
		case 6:
			return this->F;
		case 8:
			return this->PC;
		case 9:
			return this->SW;
		default:
			return "ERROR";
	}
}

void Registers::Load(string str, int n) {
	switch (n) {
		case 0:
			this->A = str;
			break;
		case 1:
			this->X = str;
			break;
		case 2:
			this->L = str;
			break;
		case 3:
			this->B = str;
			break;
		case 4:
			this->S = str;
			break;
		case 5:
			this->T = str;
			break;
		case 6:
			this->F = str;
			break;
		case 8:
			this->PC = str;
			break;
		case 9:
			this->SW = str;
			break;
	}
}

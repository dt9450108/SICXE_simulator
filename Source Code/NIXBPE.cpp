#include "NIXBPE.h"

NIXBPE::NIXBPE() {
	// TODO Auto-generated constructor stub
	this->n = false;
	this->i = false;
	this->x = false;
	this->b = false;
	this->p = false;
	this->e = false;
}

NIXBPE::~NIXBPE() {
	// TODO Auto-generated destructor stub
}

//find n i x b p e bits
// parameter a string of two hex character
void NIXBPE::decodeNI(char ch_hex) {
    string ded = toBincode(ch_hex);
    this->n = (ded[2] - '0' > 0) ? true : false;
    this->i = (ded[3] - '0' > 0) ? true : false;
}

void NIXBPE::decodeXBPE(char ch_hex) {
    string ded = toBincode(ch_hex);
    this->x = (ded[0] - '0' > 0) ? true : false;
    this->b = (ded[1] - '0' > 0) ? true : false;
    this->p = (ded[2] - '0' > 0) ? true : false;
    this->e = (ded[3] - '0' > 0) ? true : false;
}

void NIXBPE::SetNbit(bool n) {
    this->n = n;
}

void NIXBPE::SetIbit(bool i) {
    this->i = i;
}

void NIXBPE::SetXbit(bool x) {
    this->x = x;
}

void NIXBPE::SetBbit(bool b) {
    this->b = b;
}

void NIXBPE::SetPbit(bool p) {
    this->p = p;
}

void NIXBPE::SetEbit(bool e) {
    this->e = e;
}

bool NIXBPE::GetNbit() {
    return this->n;
}

bool NIXBPE::GetIbit() {
    return this->i;
}

bool NIXBPE::GetXbit() {
    return this->x;
}

bool NIXBPE::GetBbit() {
    return this->b;
}

bool NIXBPE::GetPbit() {
	return this->p;
}
bool NIXBPE::GetEbit() {
	return this->e;
}

#ifndef NIXBPE_H_
#define NIXBPE_H_

#include <iostream>
#include <string>
#include "Specification.h"
using std::string;
using std::cout;
using std::endl;

class NIXBPE {
private:
	bool n;
	bool i;
	bool x;
	bool b;
	bool p;
	bool e;
public:
	NIXBPE();
	virtual ~NIXBPE();

	void decodeNI(char ch_hex);
	void decodeXBPE(char ch_hex);

	void SetNbit(bool n);
	void SetIbit(bool i);
	void SetXbit(bool x);
	void SetBbit(bool b);
	void SetPbit(bool p);
	void SetEbit(bool e);

	bool GetNbit();
	bool GetIbit();
	bool GetXbit();
	bool GetBbit();
	bool GetPbit();
	bool GetEbit();
};

#endif /* NIXBPE_H_ */

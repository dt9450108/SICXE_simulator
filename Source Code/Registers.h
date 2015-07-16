#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <string>
using std::string;

class Registers {
private:
	string A;
	string X;
	string L;
	string B;
	string S;
	string T;
	string F;
	string PC;
	string SW;
public:
	Registers();
	virtual ~Registers();

	//using number to access register content
	string Store(int n) const;

	//A = 0, X = 1, L = 2, B = 3, S = 4, T = 5, F = 6, PC = 8, SW = 9
	void Load(string str, int n);
};

#endif /* REGISTERS_H_ */

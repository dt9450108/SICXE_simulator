#ifndef SPECIFICATION_H_
#define SPECIFICATION_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

// four kinds of Record in object program
typedef struct HeadRecord {
	string name;
	int startAddr;
	int size;
} myHeadRd;

typedef struct TextRecord {
	int startAddr;
	// size in byte
	int size;
	string record;
} myTextRd;

typedef struct ModificationRecord {
	int startAddr;
	int length;
} myModificationRd;

typedef struct EndRecord {
	int executionAddr;
} myEndRd;

typedef struct ObjectCode {
	int format;
	int opNum;
	string opStr;
	string data;
} myObjectCode;

enum {
	A = 0, X = 1, L = 2, B = 3, S = 4, T = 5, F = 6, PC = 8, SW = 9
};

int toUnint(string str);
int toInt(string str);
string toHex(int n, int digit);
string toBincode(int n);
string toBincode(char ch);
int htd(char ch);
char dth(int num);
string LookUpTable(string str, int num, bool type, int offset, int *format, int *opNum);

#endif /* SPECIFICATION_H_ */

#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include "Registers.h"
#include "NIXBPE.h"
#include "MemoryManager.h"
#include "Specification.h"
using namespace std;

class Executor {
private:
    MemoryManager *memBlock;
    NIXBPE *nixbpe;
    Registers *regs;
    int program_counter;
    string cur_byte;
    myObjectCode cur_objectCode;

    void operators(int op, int format);
    void fetchByte();
    string decode(int *prePCcount);
    void execute();
public:
    Executor(MemoryManager *memBlock, NIXBPE *nixbpe, Registers *regs, int firstOpAddr);
    virtual ~Executor();

    bool oneStep(string *opInstructStr, int *prePCcount);
    bool SaveFileResult(string fileName, string *errorMsg);
    int GetCurrentFormat();
};

#endif /* EXECUTOR_H_ */

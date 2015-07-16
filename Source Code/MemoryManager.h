#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <iostream>
#include <vector>
#include <string>
#include "Loader.h"
#include "Specification.h"
using namespace std;

class MemoryManager {
private:
    // object program head record program start address in byte
    int StartLoc;

    // object program load in memory address in byte
    int LoadAddr;
    int ObjectProgramSize;
    string MemoryBlock;
public:
    MemoryManager();
    virtual ~MemoryManager();

    bool LoadInMemory(Loader *loader);
    void Write(int loc, string data);
    string Read(int loc, int bytes);
    string Output();
    int getLoadAddr(void);
    string *getMemoryBlock(void);
};

#endif /* MEMORYMANAGER_H_ */

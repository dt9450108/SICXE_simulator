/*
 * MemoryManager.cpp
 *
 *  Created on: 2015年5月20日
 *      Author: Administrator
 */

#include "MemoryManager.h"
#include <iomanip>

MemoryManager::MemoryManager() {
    // TODO Auto-generated constructor stub
    this->StartLoc = 0;
    this->LoadAddr = 0;
    this->ObjectProgramSize = 0;
    this->MemoryBlock = "";
}

MemoryManager::~MemoryManager() {
    // TODO Auto-generated destructor stub
}

bool MemoryManager::LoadInMemory(Loader *loader) {
    // number of text record
    vector<myTextRd> *TextRd = (*loader).GetTextRecord();
    vector<myModificationRd> *ModifyRd = (*loader).GetModificationRecord();

    int textRdNum = (int) (*loader).GetTextRecord()->size();

    // number of reserved space of hex character
    int remainChsNum;

    // set Load to memory address
    this->LoadAddr = toUnint((*loader).getLoadAddr());
    this->StartLoc = (*loader).GetHeadRecord()->startAddr;
    this->ObjectProgramSize = (*loader).GetHeadRecord()->size;

    // number of hex characters in i text record to i + 1 text record,
    // the last element is object program size * 2,
    // where i = 0, 1, 2,...,TextRd.size()
    int *intervalNum = new int[textRdNum];
    for (int i = 0; i < textRdNum - 1; i++) {
        intervalNum[i] = (*TextRd)[i + 1].startAddr - (*TextRd)[i].startAddr;
    }
    intervalNum[textRdNum - 1] = this->ObjectProgramSize - TextRd->back().startAddr;

    for (int i = 0; i < textRdNum; i++) {
        remainChsNum = intervalNum[i] - TextRd->at(i).size;
        if (remainChsNum) {
            this->MemoryBlock += TextRd->at(i).record;

            for (int j = 0; j < remainChsNum; j++)
                this->MemoryBlock += "**";
        } else {
            this->MemoryBlock += TextRd->at(i).record;
        }
    }

    // process modification record
    string mtmpStr1, mtmpStr2;
    int tloc, naddr, llength;
    for (unsigned int i = 0; i < ModifyRd->size(); i++) {
        tloc = ModifyRd->at(i).startAddr * 2 + 1;
        llength = ModifyRd->at(i).length;
        mtmpStr1 = this->MemoryBlock.substr(tloc, llength);
        naddr = toInt(mtmpStr1) + this->LoadAddr;
        mtmpStr2 = toHex(naddr, llength);
        this->MemoryBlock.replace(tloc, llength, mtmpStr2);
    }

    delete[] intervalNum;
    return true;
}

void MemoryManager::Write(int loc, string data) {
    int DATA_SIZE = data.length();
    loc = (loc - this->LoadAddr) * 2;

    for (int i = 0, j = loc; i < DATA_SIZE; i++, j++)
        this->MemoryBlock[j] = data[i];

}

string MemoryManager::Read(int loc, int bytes) {
    int fetchLength = bytes * 2;
    int nloc = (loc - this->LoadAddr) * 2;
    string fetchStr = this->MemoryBlock.substr(nloc, fetchLength);
    return fetchStr;
}

int MemoryManager::getLoadAddr(void) {
    return this->LoadAddr;
}

string *MemoryManager::getMemoryBlock(void) {
    return &this->MemoryBlock;
}

string MemoryManager::Output() {
	int length = this->MemoryBlock.length(), number, index, loopNum;
	int k = this->LoadAddr;
	string tmpAddr, outputStr;

	outputStr = "Address                           Contents\n";

	loopNum = (((length / 50.0) - (double) (length / 50)) == 0.0) ? ((int) length / 50) : ((int) length / 50 + 1);

	index = 0;
	for (int i = 0; i < loopNum; i++) {
		tmpAddr = toHex(index / 2 + k, 6);
		outputStr += tmpAddr + "     ";

		if (i < loopNum - 1) {
			for (int j = 0; j < 4; j++) {
				outputStr += this->MemoryBlock.substr(index, 10) + " ";
				index += 10;
			}
			outputStr += this->MemoryBlock.substr(index, 10) + "\n";
			index += 10;
		} else {
			int remainLength = length - index;
			int remainLoopNum = (((remainLength / 10.0) - (double) (remainLength / 10)) == 0.0) ? ((int) remainLength / 10) : ((int) remainLength / 10 + 1);
			for (int j = 0; j < remainLoopNum; j++) {
				number = (length - index >= 10) ? 10 : (length - index);
				if (j < remainLoopNum - 1) {
					outputStr += this->MemoryBlock.substr(index, number) + " ";
					index += 10;
				} else {
					outputStr += this->MemoryBlock.substr(index, number);
				}
			}
		}
	}
	return outputStr;
}

/*
 * Loader.cpp
 *
 *  Created on: 2015年5月19日
 *      Author: Administrator
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Loader.h"
#include "Specification.h"
using namespace std;

Loader::Loader() {
    // TODO Auto-generated constructor stub
    this->LoadAddr = "";
}

Loader::~Loader() {
    // TODO Auto-generated destructor stub
}

bool Loader::LoadObjectProgram(string FILE_PATH_NAME, string LoadAddr) {
    fstream fop;
    fop.open(FILE_PATH_NAME.c_str(), ios::in);
    if (!fop) {
        cerr << "Cannot open the file : \n" << FILE_PATH_NAME << endl;
        return false;
    }

    vector<string> DataFromFile;
    string tmpStr;
    while (getline(fop, tmpStr))
        DataFromFile.push_back(tmpStr);
    fop.close();

    myTextRd tmpText;
    myModificationRd tmpModify;

    for (unsigned int i = 0; i < DataFromFile.size(); i++) {
        string thisStr = DataFromFile.at(i);
        switch (thisStr.at(0)) {
            case 'H':
            case 'h':
                HeadRecord.name = thisStr.substr(1, 6);
                HeadRecord.startAddr = toUnint(thisStr.substr(7, 6));
                HeadRecord.size = toUnint(thisStr.substr(13, 6));
                break;
            case 'T':
            case 't':
                tmpText.startAddr = toUnint(thisStr.substr(1, 6));
                tmpText.size = toUnint(thisStr.substr(7, 2));
                tmpText.record = thisStr.substr(9, 60);
                TextRecord.push_back(tmpText);
                break;
            case 'M':
            case 'm':
                tmpModify.startAddr = toUnint(thisStr.substr(1, 6));
                tmpModify.length = toUnint(thisStr.substr(7, 2));
                ModificationRecord.push_back(tmpModify);
                break;
            case 'E':
            case 'e':
                EndRecord.executionAddr = toUnint(thisStr.substr(1, 6));
                break;
            default:
                cerr << "Data from file have some error\n";
                return false;
        }
    }

    this->LoadAddr = LoadAddr;

    return true;
}

myHeadRd *Loader::GetHeadRecord(void) {
    return &(this->HeadRecord);
}

vector<myTextRd> *Loader::GetTextRecord(void) {
    return &(this->TextRecord);
}

vector<myModificationRd> *Loader::GetModificationRecord(void) {
    return &(this->ModificationRecord);
}

myEndRd *Loader::GetEndRecord(void) {
    return &(this->EndRecord);
}

string Loader::getLoadAddr(void) {
    return this->LoadAddr;
}

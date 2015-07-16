#ifndef LOADER_H_
#define LOADER_H_

#include <vector>
#include <string>
#include "Specification.h"
using std::vector;
using std::string;

class Loader {
private:
    myHeadRd HeadRecord;
    vector<myTextRd> TextRecord;
    vector<myModificationRd> ModificationRecord;
    myEndRd EndRecord;
    string LoadAddr;
public:
    Loader();
    virtual ~Loader();

    bool LoadObjectProgram(string FILE_PATH_NAME, string LoadAddr);
    myHeadRd *GetHeadRecord(void);
    vector<myTextRd> *GetTextRecord(void);
    vector<myModificationRd> *GetModificationRecord(void);
    myEndRd *GetEndRecord(void);
    string getLoadAddr(void);
};

#endif /* LOADER_H_ */

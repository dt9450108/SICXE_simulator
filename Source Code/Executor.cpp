#include "Executor.h"

Executor::Executor(MemoryManager *memBlock, NIXBPE *nixbpe, Registers *regs, int firstOpAddr) {
    // TODO Auto-generated constructor stub
    this->memBlock = memBlock;
    this->nixbpe = nixbpe;
    this->regs = regs;
    this->program_counter = firstOpAddr + this->memBlock->getLoadAddr();
    this->regs->Load(toHex(firstOpAddr, 6), PC);
}

Executor::~Executor() {
    // TODO Auto-generated destructor stub
    this->memBlock = NULL;
    this->nixbpe = NULL;
    this->regs = NULL;
}

bool Executor::oneStep(string *opInstructStr, int *prePCcount) {
    if (!(this->program_counter + 1)) {
        return true;
    } else {
        fetchByte();
        *opInstructStr = decode(prePCcount);
        execute();
    }
    return false;
}

void Executor::fetchByte() {
    this->cur_byte = this->memBlock->Read(this->program_counter, 1);
}

string Executor::decode(int *prePCcount) {
    // first decode n i
    this->nixbpe->decodeNI(this->cur_byte[1]);

    // cal n i offset
    int offset = 0;
    if (this->nixbpe->GetNbit())
        offset += 2;
    if (this->nixbpe->GetIbit())
        offset += 1;

    // look up op table to find instruction string
    int curFormat, curOpNum;
    string opStr = LookUpTable(this->cur_byte, 0, true, offset, &curFormat, &curOpNum);
    string reFetch;

    // if find current op code
    if (curFormat) {
        this->cur_objectCode.format = curFormat;
        this->cur_objectCode.opNum = curOpNum;
        this->cur_objectCode.opStr = opStr;
        switch (curFormat) {
            case 1:
                this->cur_objectCode.data = "";
                break;
            case 2:
                reFetch = this->memBlock->Read(this->program_counter, 2);
                this->cur_objectCode.data = reFetch.substr(2, 2);
                break;
            case 3:
                char xbpe = this->memBlock->Read(this->program_counter, 2).at(2);
                this->nixbpe->decodeXBPE(xbpe);

                if (!this->nixbpe->GetEbit()) {
                    reFetch = this->memBlock->Read(this->program_counter, 3);
                    this->cur_objectCode.data = reFetch.substr(3, 3);
                } else {
                    curFormat = 4;
                    this->cur_objectCode.format = curFormat;
                    this->cur_objectCode.opStr = "+" + opStr;
                    reFetch = this->memBlock->Read(this->program_counter, 4);
                    this->cur_objectCode.data = reFetch.substr(3, 5);
                }
                break;
        }
        *prePCcount = this->program_counter;
        this->program_counter += curFormat;
        this->regs->Load(toHex(this->program_counter, 6), PC);
    } else {
        cerr << "CANNOT FOUND OP CODE WITH : " << this->cur_byte << endl;
    }

    return reFetch;
}

void Executor::execute() {
    int op = this->cur_objectCode.opNum;
    int format = this->cur_objectCode.format;
    this->operators(op, format);
}

void Executor::operators(int op, int format) {
    bool n, i, x, b, p;
    n = this->nixbpe->GetNbit();
    i = this->nixbpe->GetIbit();
    x = this->nixbpe->GetXbit();
    b = this->nixbpe->GetBbit();
    p = this->nixbpe->GetPbit();

    int reg1, reg2, ta = 0, memAddr;
    string disp, addr, operand;

    // according op format to decides the content of Target Address
    switch (format) {
        case 1:
            break;
        case 2:
            reg1 = this->cur_objectCode.data[0] - '0';
            reg2 = this->cur_objectCode.data[1] - '0';
            break;
        case 3:
            disp = this->cur_objectCode.data;

            // if immediate addressing
            // else if indirect addressing
            // eles if simple addressing
            if (!n && i) {
                // if use pc addressing
                // if use base addressing
                if (p)
                    ta += this->program_counter;
                else if (b)
                    ta += toUnint(this->regs->Store(B));

                ta += toInt(disp);
                operand = toHex(ta, 6);
                memAddr = ta;
            } else if (n && !i) {
                int tmpOperand;

                // if use pc addressing
                // if use base addressing
                // if not pc and base addressing, add offset
                if (p)
                    ta += this->program_counter;
                else if (b)
                    ta += toUnint(this->regs->Store(B));
                else
                    ta += this->memBlock->getLoadAddr();

                ta += toInt(disp);
                operand = this->memBlock->Read(ta, 3);
                tmpOperand = toUnint(operand);
                operand = this->memBlock->Read(tmpOperand, 3);
                memAddr = tmpOperand;
            } else if (n && i) {
                // if use index addressing
                if (x)
                    ta += toUnint(this->regs->Store(X));

                // if use pc addressing
                // if use base addressing
                // if not pc and base addressing, add offset
                if (p)
                    ta += this->program_counter;
                else if (b)
                    ta += toUnint(this->regs->Store(B));
                else
                    ta += this->memBlock->getLoadAddr();

                ta += toInt(disp);
                operand = this->memBlock->Read(ta, 3);
                memAddr = ta;
            } else {
                cerr << "NO SIC COMPATIBLE FUNCTION\n";
                return;
            }
            break;
        case 4:
            addr = this->cur_objectCode.data;

            // if immediate addressing
            // else if indirect addressing
            // eles if simple addressing
            if (!n && i) {
                ta += toUnint(addr);
                operand = toHex(ta, 6);
                memAddr = ta;
            } else if (n && !i) {
                int tmpOperand;

                ta += toUnint(addr);
                operand = this->memBlock->Read(ta, 3);
                tmpOperand = toUnint(operand);
                operand = this->memBlock->Read(tmpOperand, 3);
                memAddr = tmpOperand;
            } else if (n && i) {
                // if use index addressing
                if (x)
                    ta += toUnint(this->regs->Store(X));

                ta += toUnint(addr);
                operand = this->memBlock->Read(ta, 3);
                memAddr = ta;
            } else {
                cerr << "ADDREESING IN FORAMT 4 HAVE EXPECTED ERROR\n";
                return;
            }
            break;
        default:
            cerr << "CANNOT FIND THIS FORMAT OPCODE\n";
    }

    // variables in switch statement used
    int tmp1, tmp2;
    string tmpStr1, tmpStr2, compCode;
    switch (op) {
        case 144:
            // format : 2, ADDR r1,r2
            // r2 <- (r2) + (r1)
            tmp1 = toInt(this->regs->Store(reg1));
            tmp2 = toInt(this->regs->Store(reg2));
            tmp2 += tmp1;
            this->regs->Load(toHex(tmp2, 6), reg2);
            break;
        case 148:
            // format : 2, SUBR r1,r2
            // r2 <- (r2) ¡V (r1)
            tmp1 = toInt(this->regs->Store(reg1));
            tmp2 = toInt(this->regs->Store(reg2));
            tmp2 -= tmp1;
            this->regs->Load(toHex(tmp2, 6), reg2);
            break;
        case 152:
            // format : 2, MULR r1,r2
            // r2 <- (r2) * (r1)
            tmp1 = toInt(this->regs->Store(reg1));
            tmp2 = toInt(this->regs->Store(reg2));
            tmp2 *= tmp1;
            this->regs->Load(toHex(tmp2, 6), reg2);
            break;
        case 156:
            // format : 2, DIVR r1,r2
            // r2 <- (r2) / (r1)
            tmp1 = toInt(this->regs->Store(reg1));
            tmp2 = toInt(this->regs->Store(reg2));
            if (!tmp1) {
                cerr << "CANNOT DIVIDE BY ZERO\n";
                return;
            }
            tmp2 /= tmp1;
            this->regs->Load(toHex(tmp2, 6), reg2);
            break;
        case 160:
            // format : 2, COMPR r1,r2
            // (r1) : (r2)
            tmp1 = toInt(this->regs->Store(reg1));
            tmp2 = toInt(this->regs->Store(reg2));
            compCode = (tmp1 == tmp2) ? "0000C0" : (tmp1 > tmp2 ? "000080" : "000040");
            this->regs->Load(compCode, SW);
            break;
        case 172:
            // format : 2, RMO r1,r2
            // r2 <- (r1)
            this->regs->Load(this->regs->Store(reg1), reg2);
            break;
        case 180:
            // format : 2, CLEAR r1
            // r1 <- 0
            this->regs->Load("000000", reg1);
            break;
        case 184:
            // format : 2, TIXR r1
            // X <- (X) + 1; (X) : (r1)
            tmp1 = toInt(this->regs->Store(X));
            tmp2 = toInt(this->regs->Store(reg1));
            tmp1++;
            this->regs->Load(toHex(tmp1, 6), X);

            compCode = (tmp1 == tmp2) ? "0000C0" : (tmp1 > tmp2 ? "000080" : "000040");
            this->regs->Load(compCode, SW);
            break;
        case 0:
            // format : 3/4, LDA m
            // A <- (m..m+2)
            this->regs->Load(operand, A);
            break;
        case 4:
            // format : 3/4, LDX m
            // X <- (m..m+2)
            this->regs->Load(operand, B);
            break;
        case 8:
            // format : 3/4, LDL m
            // L <- (m..m+5)
            this->regs->Load(operand, L);
            break;
        case 12:
            // format : 3/4, STA m
            // m..m+2 <- (A)
            tmpStr1 = this->regs->Store(A);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 16:
            // format : 3/4, STX m
            // m..m+2 <- (X)
            tmpStr1 = this->regs->Store(X);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 20:
            // format : 3/4, STL m
            // m..m+2 <- (L)
            tmpStr1 = this->regs->Store(L);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 24:
            // format : 3/4, ADD m
            // A <- (A)+(m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);
            tmp2 += tmp1;
            this->regs->Load(toHex(tmp2, 6), A);
            break;
        case 28:
            // format : 3/4, SUB m
            // A <- (A) ¡V (m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);
            tmp1 -= tmp2;
            this->regs->Load(toHex(tmp1, 6), A);
            break;
        case 32:
            // format : 3/4, MUL m
            // A <- (A) * (m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);
            tmp1 *= tmp2;
            this->regs->Load(toHex(tmp1, 6), A);
            break;
        case 36:
            // format : 3/4, DIV m
            // A <- (A) / (m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);
            if (!tmp2) {
                cerr << "CANNOT DIVIDE BY ZERO\n";
                return;
            }
            tmp1 /= tmp2;
            this->regs->Load(toHex(tmp1, 6), A);
            break;
        case 40:
            // format : 3/4, COMP m
            // (A) : (m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);

            compCode = (tmp1 == tmp2) ? "0000C0" : (tmp1 > tmp2 ? "000080" : "000040");
            this->regs->Load(compCode, SW);
            break;
        case 44:
            // format : 3/4, TIX m
            // X <- (X) + 1; (X) : (m..m+2)
            tmp1 = toInt(this->regs->Store(X));
            tmp2 = toInt(operand);
            tmp1++;
            this->regs->Load(toHex(tmp1, 6), X);

            compCode = (tmp1 == tmp2) ? "0000C0" : (tmp1 > tmp2 ? "000080" : "000040");
            this->regs->Load(compCode, SW);
            break;
        case 48:
            // format : 3/4, JEQ m
            // PC <- m if CC set to =
            compCode = this->regs->Store(SW);
            if (!(compCode[4] - 'C')) {
                this->program_counter = memAddr;
                this->regs->Load(toHex(memAddr, 6), PC);
            }
            break;
        case 52:
            // format : 3/4, JGT m
            // PC <- m if CC set to >
            compCode = this->regs->Store(SW);
            if (!(compCode[4] - '8')) {
                this->program_counter = memAddr;
                this->regs->Load(toHex(memAddr, 6), PC);
            }
            break;
        case 56:
            // format : 3/4, JLT m
            // PC <- m if CC set to <
            compCode = this->regs->Store(SW);
            if (!(compCode[4] - '4')) {
                this->program_counter = memAddr;
                this->regs->Load(toHex(memAddr, 6), PC);
            }
            break;
        case 60:
            // format : 3/4, J m
            // PC <- m
            this->program_counter = memAddr;
            this->regs->Load(toHex(memAddr, 6), PC);
            break;
        case 64:
            // format : 3/4, AND m
            // A <- (A) & (m..m+2)
            tmp1 = toInt(this->regs->Store(A));
            tmp2 = toInt(operand);
            tmp2 = tmp1 & tmp2;
            this->regs->Load(toHex(tmp2, 6), A);
            break;
        case 72:
            // format : 3/4, JSUB m
            // L <- (PC); PC <- m
            this->regs->Load(toHex(this->program_counter, 6), L);
            this->program_counter = memAddr;
            this->regs->Load(toHex(memAddr, 6), PC);
            break;
        case 76:
            // format : 3/4, RSUB
            // PC <- (L)
            tmp1 = toInt(this->regs->Store(L));
            this->program_counter = tmp1;
            this->regs->Load(toHex(this->program_counter, 6), PC);
            break;
        case 80:
            // format : 3/4, LDCH m
            // A[rightmost byte] <- (m)
            tmpStr1 = this->regs->Store(A);
            tmpStr2 = operand.substr(0, 2);
            tmpStr1 = tmpStr1.substr(0, 4) + tmpStr2;
            this->regs->Load(tmpStr1, A);
            break;
        case 84:
            // format : 3/4, STCH m
            // m <- (A) [rightmost byte]
            tmpStr1 = this->regs->Store(A).substr(4, 2);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 104:
            // format : 3/4, LDB m
            // B <- (m..m+2)
            this->regs->Load(operand, B);
            break;
        case 108:
            // format : 3/4, LDS m
            // S <- (m..m+2)
            this->regs->Load(operand, S);
            break;
        case 116:
            // format : 3/4, LDT m
            // T <- (m..m+2)
            this->regs->Load(operand, T);
            break;
        case 120:
            // format : 3/4, STB m
            // m..m+2 <- (B)
            tmpStr1 = this->regs->Store(B);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 124:
            // format : 3/4, STS m
            // m..m+2 <- (S)
            tmpStr1 = this->regs->Store(S);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 132:
            // format : 3/4, STT m
            // m..m+2 <- (T)
            tmpStr1 = this->regs->Store(T);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        case 232:
            // format : 3/4, STSW m
            // m..m+2 <- (SW)
            tmpStr1 = this->regs->Store(SW);
            this->memBlock->Write(memAddr, tmpStr1);
            break;
        default:
            cerr << "EXECUTE OPERATER ENCOUNTER SOME ERROR" << endl;
//			out = "ERROR";
            break;
    }
}

bool Executor::SaveFileResult(string fileName, string *errorMsg) {
    fstream fout;
    fout.open(fileName.c_str(), ios::out);
    if (!fout) {
        *errorMsg = "Saving File expected end with file : " + fileName;
        return false;
    }

    int i = 1;
    int ignoren;
    string ignore;
    while (!this->oneStep(&ignore, &ignoren)) {
        fout << "STEP  : =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= " << setw(4) << (i++) << endl;
        fout << "CURRENT INSTRUCTION  : " << this->cur_objectCode.opStr + " " + this->cur_objectCode.data << endl;
        fout << "PC   : " << this->regs->Store(PC) << "    A   : " << this->regs->Store(A) << "    X : " << this->regs->Store(X) << endl;
        fout << "L    : " << this->regs->Store(L) << "    B   : " << this->regs->Store(B) << "    S : " << this->regs->Store(S) << endl;
        fout << "T    : " << this->regs->Store(T) << "    SW  : " << this->regs->Store(SW) << "\n\n";
    }
    fout.close();

    return true;
}

int Executor::GetCurrentFormat() {
    return this->cur_objectCode.format;
}

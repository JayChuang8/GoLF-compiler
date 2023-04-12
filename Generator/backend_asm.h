#ifndef backend_asm_h
#define backend_asm_h

#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include "../Utility/utility.h"
#include "../Parser/ast.h"
using namespace std;

class BackendASM
{
public:
    BackendASM(Utility &util);

    void pass1_cb(AST *node);
    void pass1_post_cb(AST *node);
    void pass2_cb(AST *node);
    void pass2_post_cb(AST *node);
    void pass3_cb(AST *node);
    void pass3_post_cb(AST *node);
    void pass4_cb(AST *node);
    void gen(AST &ast);

private:
    Utility &util;
    string allocreg();
    void freereg(string reg);
    string allocArgReg();
    void freeArgReg(string reg);
    void emitlabel(string label);
    void emit(string instr);
    void prologue();
    void epilogue();
    string id2asm(string name);
    string getlabel();

    string getDataLabel();

    set<string> pool;
    set<string> argPool;
    int labelnum;
    std::unordered_map<std::string, std::string> OP2ASM;
};

#endif
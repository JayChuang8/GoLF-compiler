#ifndef semantic_h
#define semantic_h

#include <unordered_map>
#include "../Utility/utility.h"
#include "../Parser/ast.h"
#include "stab.h"

class Semantic
{
public:
    Semantic(Utility &util, SymbolTable &stab);

    void pass1_cb(AST *node);
    void pass2_cb(AST *node);
    void pass2_post_cb(AST *node);
    void pass3_cb(AST *node);
    void pass4_cb(AST *node);
    void pass4_post_cb(AST *node);
    bool typecheckok(AST *node);
    void semantic(AST &ast);

private:
    Utility &util;
    SymbolTable &stab;
};

#endif

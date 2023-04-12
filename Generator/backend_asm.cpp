#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "./backend_asm.h"
using namespace std;

BackendASM::BackendASM(Utility &util) : util(util),
                                        pool({"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"}),
                                        argPool({"$a0", "$a1", "$a2", "$a3", "$a4", "$a5", "$a6", "$a7", "$a8", "$a9"}),
                                        labelnum(0),
                                        OP2ASM({{"+", "addu"},
                                                {"-", "sub"},
                                                {"*", "mul"},
                                                {"/", "div"},
                                                {"%", ""}, //
                                                {"==", "seteq"},
                                                {"!=", "setne"},
                                                {"<", "slt"},  //
                                                {"<=", "sle"}, //
                                                {">", "sgt"},  //
                                                {">=", "sge"}, //
                                                {"&&", ""},    //
                                                {"||", ""}})   //
{
}

string BackendASM::allocreg()
{
    if (pool.empty())
    {
        util.error("expression too complicated", 0);
    }
    string reg = *pool.begin();
    pool.erase(pool.begin());
    return reg;
}

void BackendASM::freereg(string reg)
{
    if (pool.find(reg) != pool.end())
    {
        util.error("attempting to free unallocated register", 0);
    }
    else
    {
        // cout << "---REG FREED--- " + reg << endl;
        pool.insert(reg);
    }
}

string BackendASM::allocArgReg()
{
    if (argPool.empty())
    {
        cout << "expression too complicated" << endl;
        exit(1);
    }
    string reg = argPool.front();
    argPool.erase(argPool.begin());
    return reg;
}

void BackendASM::freeArgReg(string reg)
{
    for (const string &str : argPool)
    {
        if (str == reg)
        {
            cout << "attempting to free unallocated register" << endl;
            exit(1);
        }
        else
        {
            argPool.push_back(reg);
        }
    }
}

void BackendASM::emitlabel(string label)
{
    cout << label << ":" << endl;
}

void BackendASM::emit(string instr)
{
    cout << "\t" << instr << endl;
}

void BackendASM::prologue()
{
    emit("Ltrue = 1");
    emit("Lfalse = 0");
    emit(".globl main");
    emit(".data");
}

void BackendASM::epilogue()
{
    emit("j Lhalt");
}

string BackendASM::id2asm(string name)
{
    // prefix with "L" so it doesn't clash with assembler's reserved names
    return "L" + name;
}

int dataLabelNum = 0;
string BackendASM::getDataLabel()
{
    // eg. strings
    // denote label that represents location in the code where data is stored
    return "S" + to_string(dataLabelNum++);
}

int labelNum = 0;
string BackendASM::getlabel()
{
    // eg. functions
    // denote label that represents a location in the code where control flow may change
    return "L" + to_string(labelNum++);
}

void BackendASM::pass1_cb(AST *node)
{
    if (node->type == "string")
    {
        node->reg = getDataLabel();
        emitlabel(node->reg);
        for (int i = 0; i < node->attribute.size(); i++)
        {
            int currentByte = static_cast<int>(node->attribute[i]);
            if (currentByte == 92 && i < node->attribute.size() - 1 && static_cast<int>(node->attribute[i + 1]) == 110)
            {
                // for \n
                emit(".byte 10");
                i++;
            }
            else
            {
                emit(".byte " + to_string(currentByte));
            }
        }
        emit(".byte 0");
    }
}

void BackendASM::pass2_cb(AST *node)
{
    if (node->type == "func")
    {
        node->sym->rtname = "L" + node->kids[0].attribute;
        node->sym->allocspace = (1 +
                                 node->countChildrenByType(*node, "var") +
                                 node->countChildrenByType(*node, "formal")) *
                                4;
        if (node->kids[0].attribute == "main")
        {
            emitlabel("main");
            emit("jal " + node->sym->rtname);
            epilogue();
        }
    }
    else if (node->type == "globvar")
    {
        node->sym->rtname = "G" + node->kids[0].attribute;
        emitlabel(node->sym->rtname);
        emit(".word 0");
    }
}

int currentStackAddress = 0;

void BackendASM::pass3_cb(AST *node)
{
    if (node->type == "func")
    {
        emitlabel(node->sym->rtname);
        emit("subu $sp, $sp, " + to_string(node->sym->allocspace));
        // currentStackAddress = node->sym->allocspace;
        // emit return register address
        emit("sw $ra, 0($sp)");

        // make sure all the child nodes in block are defined
        node->kids[2].prepost([this](AST *node)
                              { pass3_cb(node); },
                              [this](AST *node)
                              { pass3_post_cb(node); });

        // emit return label for func
        emitlabel("R" + node->sym->rtname);
        emit("lw $ra,0($sp)");
        emit("addu $sp,$sp," + to_string(node->sym->allocspace));
        emit("jr $ra");
        node->prune();
    }
    else if (node->type == "funccall")
    {
        // make sure all the right child nodes (actuals) are defined
        node->kids[1].prepost([this](AST *node)
                              { pass3_cb(node); },
                              [this](AST *node)
                              { pass3_post_cb(node); });

        // move all needed reg's into arg reg's
        for (AST &child : node->kids[1].kids)
        {
            // string reg = child.reg;
            // if (child.sym != nullptr)
            //     reg = child.sym->reg;

            emit("move " + allocArgReg() + ", " + child.reg);
            freeArgReg(child.reg);
        }

        // function call
        emit("jal " + node->kids[0].sym->rtname);
        node->prune();
        currentStackAddress = 0;
    }
    else if (node->type == "=")
    {
        // make sure the nodes on the RHS of assignment (=) are defined
        node->kids[1].prepost([this](AST *node)
                              { pass3_cb(node); },
                              [this](AST *node)
                              { pass3_post_cb(node); });

        string reg = node->kids[0].reg;
        if (node->kids[0].sym != nullptr)
            reg = node->kids[0].sym->rtname;
        if (node->kids[0].sym->rtname.empty())
            reg = node->kids[0].sym->reg;
        emit("sw " + node->kids[1].reg + ", " + reg);
        freereg(node->kids[1].reg);
        node->prune();
    }
    else if (node->type == "for")
    {
        string loopStart = getlabel() + "for";
        emitlabel(loopStart);

        node->kids[0].prepost([this](AST *node)
                              { pass3_cb(node); },
                              [this](AST *node)
                              { pass3_post_cb(node); });

        string loopEnd = getlabel() + "for";
        emit("beqz " + node->kids[0].reg + ", " + loopEnd);
        freereg(node->kids[0].reg);

        node->kids[1].prepost([this](AST *node)
                              { pass3_cb(node); },
                              [this](AST *node)
                              { pass3_post_cb(node); });
        emit("j " + loopStart);
        emitlabel(loopEnd);
        node->prune();
    }
    else if (node->type == "var")
    {
        currentStackAddress += 4;
        emit("sw $0, " + to_string(currentStackAddress) + "($sp)");
        node->reg = to_string(currentStackAddress) + "($sp)";
        node->sym->reg = to_string(currentStackAddress) + "($sp)";
    }
}

void BackendASM::pass3_post_cb(AST *node)
{
    if (node->type == "string")
    {
        string strLabel = node->reg;
        node->reg = allocreg();
        emit("la " + node->reg + ", " + strLabel);
    }
    else if (node->type == "int")
    {
        node->reg = allocreg();
        emit("li " + node->reg + ", " + node->attribute);
    }
    else if (node->attribute == "true" || node->attribute == "false")
    {
        node->reg = allocreg();
        cout << "[LOADB] " + node->reg + " = " + node->sym->rtname << endl;
    }
    else if (node->type == "u!")
    {
        node->reg = node->kids[0].reg;
        cout << "[NOT] " + node->reg + " = not " + node->kids[0].reg << endl;
    }
    else if (node->type == "id")
    {
        // still needs work
        node->reg = allocreg();
        string reg = node->sym->reg;
        if (node->sym->reg.empty())
            reg = node->sym->rtname;
        emit("lw " + node->reg + ", " + reg);
    }
    else if (OP2ASM.find(node->type) != OP2ASM.end())
    {
        // binary operator
        node->reg = allocreg();
        string op = OP2ASM[node->type];

        emit(op + " " + node->reg + ", " + node->kids[0].reg + ", " + node->kids[1].reg);

        freereg(node->kids[0].reg);
        freereg(node->kids[1].reg);
    }
}

void BackendASM::gen(AST &ast)
{
    // pass 1: define create .data section with strings
    prologue();
    ast.preorder([this](AST *node)
                 { pass1_cb(node); });

    // pass 2: define MIPS labels for globvar's, func's, strings
    emit(".text");
    ast.preorder([this](AST *node)
                 { pass2_cb(node); });

    // pass 3: define code for main, globvar's, and func's
    ast.prepost(
        [this](AST *node)
        { pass3_cb(node); },
        [this](AST *node)
        { pass3_post_cb(node); });

    // // pass 3:
    // ast.prepost(
    //     [this](AST *node)
    //     { pass3_cb(node); },
    //     [this](AST *node)
    //     { pass3_post_cb(node); });

    // // pass 4: define data (S#) labels
    // ast.preorder([this](AST *node)
    //              { pass4_cb(node); });

    emitlabel("Lhalt");
    emit("li $v0, 10");
    emit("syscall");
}

// void BackendASM::pass1_cb(AST *node)
// {
//     if (node->type == "if")
//     {
//         string bottom = getlabel();
//         node->kids[0].prepost([this](AST *node)
//                               { pass1_cb(node); },
//                               [this](AST *node)
//                               { pass1_post_cb(node); });
//         emit("bf " + node->kids[0].reg + ", " + bottom);
//         freereg(node->kids[0].reg);
//         node->kids[1].prepost([this](AST *node)
//                               { pass1_cb(node); },
//                               [this](AST *node)
//                               { pass1_post_cb(node); });
//         emitlabel(bottom);
//         node->prune();
//     }
//     else if (node->type == "=")
//     {
//         node->kids[1].prepost([this](AST *node)
//                               { pass1_cb(node); },
//                               [this](AST *node)
//                               { pass1_post_cb(node); });
//         emit("move " + node->kids[1].reg + ", " + id2asm(node->attribute));
//         freereg(node->kids[1].reg);
//         node->prune();
//     }
//     else if (OP2ASM.find(node->type) != OP2ASM.end())
//     {
//         // binary operator
//         node->kids[0].prepost([this](AST *node)
//                               { pass1_cb(node); },
//                               [this](AST *node)
//                               { pass1_post_cb(node); });
//         node->kids[1].prepost([this](AST *node)
//                               { pass1_cb(node); },
//                               [this](AST *node)
//                               { pass1_post_cb(node); });
//         string op = OP2ASM[node->type];
//         string reg = allocreg();
//         emit(op + " " + node->kids[0].reg + "," + node->kids[1].reg + "," + reg);
//         freereg(node->kids[0].reg);
//         freereg(node->kids[1].reg);
//         node->reg = reg;
//     }
// }

// void BackendASM::pass1_post_cb(AST *node)
// {
//     if (node->type == "id")
//     {
//         node->reg = allocreg();
//         emit("move " + id2asm(node->attribute) + ", " + node->reg);
//     }
//     else if (node->type == "number")
//     {
//         node->reg = allocreg();
//         emit("move #" + to_string(stoi(node->attribute)) + ", " + node->reg);
//     }
//     else if (node->type == "print")
//     {
//         emit("call rtsprint(" + node->kids[0].reg + ")");
//         freereg(node->kids[0].reg);
//     }
//     else if (node->type == "u-")
//     {
//         node->reg = allocreg();
//         emit("neg " + node->kids[0].reg + ", " + node->reg);
//         freereg(node->kids[0].reg);
//     }
//     else if (OP2ASM.find(node->type) != OP2ASM.end())
//     {
//         // binary operator
//         node->reg = allocreg();
//         string op = OP2ASM[node->type];
//         emit(op + " " + node->kids[0].reg + "," + node->kids[1].reg + "," + node->reg);
//         freereg(node->kids[0].reg);
//         freereg(node->kids[1].reg);
//     }
// }

// void BackendASM::pass2_cb(AST *node)
// {
//     if (node->type == "string")
//     {
//         emitlabel(getDataLabel());

//         for (char c : node->attribute)
//         {
//             emit(".byte " + to_string(static_cast<int>(c)));
//         }
//         emit(".byte 0");
//         emit(".align 2");
//         emit(".text");
//     }
//     else if (node->type == "funccall")
//     {
//         emitlabel(getlabel());
//     }
// }

// void pass2_cb(Node* node) {
//     if (node->type == "newid") {
//         // make both ints and bools into words
//         emitlabel(id2asm(node->attr));
//         emit("word 0");
//     }
// }
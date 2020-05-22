//
// Created by Texnar on 22.05.2020.
//

#ifndef BACKEND_X86_X64_BACKEND_X86_X64_H
#define BACKEND_X86_X64_BACKEND_X86_X64_H

#include "MyTree.h"

struct backend {
    //------------------------------CONSTANTS------------------------------//
    const char TYPE_OPERATOR = 1, TYPE_NUMBER   = 2,
               TYPE_VARIABLE = 3, TYPE_FUNCTION = 4;

    enum      {OPERATOR_ADD = 1, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV,
               OPERATOR_POW,     OPERATOR_SIN, OPERATOR_COS, OPERATOR_LN };
    //------------------------------CONSTANTS------------------------------//

    Tree tree;

    void whatItIs   (Node& node);     // check what type this Node

    bool isOperator (Node& node);
    bool isNumber   (Node& node);
    bool isVariable (Node& node);

};

#endif //BACKEND_X86_X64_BACKEND_X86_X64_H

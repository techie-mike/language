//
// Created by Texnar on 22.05.2020.
//

#ifndef BACKEND_X86_X64_BACKEND_X86_X64_H
#define BACKEND_X86_X64_BACKEND_X86_X64_H

#include "MyTree.h"
#include "NameTable.h"

struct _backend {
private:
    //------------------------------CONSTANTS------------------------------//
    static const char TYPE_OPERATOR = 1, TYPE_NUMBER   = 2,
                      TYPE_VARIABLE = 3, TYPE_FUNCTION = 4;

    enum {OPERATOR_ADD = 1, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV,
          OPERATOR_POW,     OPERATOR_SIN, OPERATOR_COS, OPERATOR_LN };

              static const long long  CONVERSION_FACTOR_LONG   = 100;
    constexpr static const double     CONVERSION_FACTOR_DOUBLE = (const double) CONVERSION_FACTOR_LONG;


    static const size_t DEFAULT_LENGTH_BIN_CODE = 500000;
    //------------------------------CONSTANTS------------------------------//

public:
    static Tree tree;

    //==============================COMPILER===============================//
    struct _compiler {  // struct for function and variable for binary compiler
        void compilingCode ();
    private:
        static char* text_obj_;

        inline void createAllFunctionLabel ();
        static int  createOneFunctionLabel (Node* node);
    } compiler;

    static nameTable functions;

    //==============================COMPILER===============================//


    //------------------------------FUNCTION-------------------------------//
    static void whatItIs     (Node* node);     // check what type this Node
    static void treeColoring (FILE* file, Node* node);

private:
    static bool isOperator (Node* node);
    static bool isNumber   (Node* node);
    static bool isVariable (Node* node);

};


#endif //BACKEND_X86_X64_BACKEND_X86_X64_H

//
// Created by Texnar on 22.05.2020.
//

#include "backend_x86_x64.h"

//---------------DEFINITION-OF-THE-STATIC-DATA-MEMBER---------------//
char*     _backend::_compiler::text_obj_;
Tree      _backend::tree;
nameTable _backend::functions;
//---------------DEFINITION-OF-THE-STATIC-DATA-MEMBER---------------//


void _backend::whatItIs (Node* node) {
    if (isOperator (node)) return;
    if (isNumber   (node)) return;
    if (isVariable (node)) return;
}

bool _backend::isOperator (Node* node)
{
//----------------------------DEFINE----------------------------//
#define OPER(str, num) if (!strcmp( #str , node->name)) {\
    node->type = TYPE_OPERATOR;\
    node->value = num;\
    return true;\
}
//----------------------------DEFINE----------------------------//

    OPER (+  , OPERATOR_ADD)
    OPER (-  , OPERATOR_SUB)
    OPER (*  , OPERATOR_MUL)
    OPER (/  , OPERATOR_DIV)
    OPER (^  , OPERATOR_POW)
    OPER (sin, OPERATOR_SIN)
    OPER (cos, OPERATOR_COS)
    OPER (ln , OPERATOR_LN )
    OPER (=  , 0)
    OPER (;  , 0)

#undef OPER
    return false;
}

bool _backend::isNumber   (Node* node) {
    double  temp_number = 0;
    int     num_read    = 0;

    sscanf (node->name, "%lg%n", &temp_number, &num_read);
    temp_number *= CONVERSION_FACTOR_DOUBLE;
    value_t number = (value_t) temp_number;

    if (strlen (node->name) == (size_t) num_read) {
        node->type  = TYPE_NUMBER;
        node->value = number;
        return true;
    }
    return false;
}

bool _backend::isVariable (Node* node) {
//----------------------------DEFINE----------------------------//
#define NAME(name_var) if (!strcmp(node->name , name_var)) {\
        return false;\
    } else
//----------------------------DEFINE----------------------------//
    NAME ("(")
    NAME (")")
    NAME ("<=")
    NAME (">=")
    NAME ("<")
    NAME (">")
    NAME ("==")
    NAME ("!=")
    NAME ("+")
    NAME ("-")
    NAME ("/")
    NAME ("*")
    NAME ("^")
    NAME ("sin")
    NAME ("cos")
    NAME ("deriv")
    NAME ("op")
    NAME ("while")
    NAME ("if")
    NAME ("if-else")
    NAME (";")
    NAME ("put")
    NAME ("get")
    NAME ("ret")
    NAME ("=")
    NAME (",")
#undef NAME

    if (*node->name == '$'){
        node->type = TYPE_FUNCTION;
        return true;
    } else {
        node->type = TYPE_VARIABLE;
        return true;
    }
}

void _backend::treeColoring (FILE* file, Node* node) {
    switch (node->type) {
        case TYPE_OPERATOR:
            fprintf(file, "style=\"filled\", fillcolor=\"lightgrey\" ");
            break;
        case TYPE_NUMBER:
            fprintf(file, "style=\"filled\", fillcolor=\"yellow\" "   );
            break;
        case TYPE_VARIABLE:
            fprintf(file, "style=\"filled\", fillcolor=\"lightblue\" ");
            break;
        default:
            break;
    }
}

void _backend::_compiler::compilingCode () {
    text_obj_ = (char*) calloc (DEFAULT_LENGTH_BIN_CODE, sizeof (char));
    if (!text_obj_) {
        printf ("Error in calloc in compiler!\n");
        abort ();
    }

    createAllFunctionLabel ();

}

void _backend::_compiler::createAllFunctionLabel () {
    tree.controlledExternalFunctionFromRoot (createOneFunctionLabel);
}

int _backend::_compiler::createOneFunctionLabel (Node* node) {
    if (!strcmp (node->name, ";"))
        return tree.BOTH_BRANCH;

    if (node->name[0] == '$') {
        if (functions.searchNameInTable (&(node->name[1])) == -1)
            functions.createNameInTable (&(node->name[1]));
        return tree.NO_BRANCH;
    }

    return tree.NO_BRANCH;
}


//
// Created by Texnar on 22.05.2020.
//

#include "backend_x86_x64.h"



void backend::whatItIs (Node& node) {
    if (isOperator (node)) return;
    if (isNumber   (node)) return;
    if (isVariable (node)) return;
}

bool backend::isOperator (Node& node)
{
//----------------------------DEFINE----------------------------//
#define OPER(str, num) if (!strcmp( #str , node.name)) {\
    node.type = TYPE_OPERATOR;\
    node.value = num;\
    return true;\
}
//----------------------------DEFINE----------------------------//

    OPER(+  , OPERATOR_ADD)
    OPER(-  , OPERATOR_SUB)
    OPER(*  , OPERATOR_MUL)
    OPER(/  , OPERATOR_DIV)
    OPER(^  , OPERATOR_POW)
    OPER(sin, OPERATOR_SIN)
    OPER(cos, OPERATOR_COS)
    OPER(ln , OPERATOR_LN )
    OPER(=  , 0)
    OPER(;  , 0)

#undef OPER
    return false;
}

bool backend::isNumber (Node& node) {
    value_t number = 0;
    int num_read = 0;
    sscanf (node.name, "%lg%n", &number, &num_read);
    if (strlen(node.name) == num_read){
        node.type = TYPE_NUMBER;
        node.value = number;
        return true;
    }
    return false;
}

bool backend::isVariable (Node& node) {
/*
#define VAR(str, num) if (!strcmp( #str , name)) {\
        one_element[index].type_ = TYPE_VARIABLE;\
        one_element[index].value_ = num;\
        return true;\
    }

    VAR(x , VARIABLE_X)
    VAR(y , VARIABLE_Y)
    VAR(z , VARIABLE_Z)
    VAR(t , VARIABLE_T)

#undef VAR
*/

//----------------------------DEFINE----------------------------//
#define NAME(name_var) if (!strcmp(node.name , name_var)) {\
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

    if (*node.name == '$'){
        node.type = TYPE_FUNCTION;
        return true;
    } else {
        node.type = TYPE_VARIABLE;
        return true;
    }
}
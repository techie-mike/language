//
// Created by Texnar on 22.05.2020.
//

#include "backend_x86_x64.h"

#include "binCommands.h"


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
    uploadDataFromTree ();

    searchMainFunctionView (root_);
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

void _backend::_compiler::uploadDataFromTree () {
    root_ = tree.getRoot  ();
    node_ = tree.getNodes ();
}

//      After this function will be porting code        //
void _backend::_compiler::searchMainFunctionView (tree_st index) {
    if (!strcmp (node_[index].name, ";")) {
        if (!strcmp (node_[node_[index].left].name, "$main")) {
            functionView (node_[index].left);
            return;
        }
        if (node_[index].right)
            searchMainFunctionView (node_[index].right);
    } else
        printf ("Error first line program\n");
}

bool _backend::_compiler::functionView (tree_st index) {
    if (node_[index].name[0] == '$') {
        nameTable variable;

        searchAllVariablesInFunctionView (&variable, node_[index].left);
        variable.num_arguments_ = variable.size_;   // deteted +1
        searchAllVariablesInFunctionView (&variable, node_[index].right);

        startFunction_0 (&variable, index);

        lineOfFunctionsView (&variable, node_[index].right);


    }

}

void _backend::_compiler::searchAllVariablesInFunctionView (nameTable* variables, tree_st index) {
    if (index) {
        if (node_[index].type == TYPE_VARIABLE &&
           !checkNameVariable (variables, node_[index].name))
            variables->createNameInTable (node_[index].name);
        searchAllVariablesInFunctionView (variables, node_[index].left );
        searchAllVariablesInFunctionView (variables, node_[index].right);
    }
}

bool _backend::_compiler::checkNameVariable (nameTable* table, char* name) {
    if (table->searchNameInTable (name) != -1)
        return true;
    else return false;
}

void _backend::_compiler::writeInObjText (const byte* command, size_t num_bytes) {
    if (text_obj_ == 0 || command == nullptr || num_bytes == 0) {
        printf ("Error in arguments of 'writeInObjText\n");
        abort ();
    }
    if (record_position_ + 10 >= DEFAULT_LENGTH_BIN_CODE) {
        printf ("Error in length of binary code, not enough space\n");
        abort ();
    }

    memcpy (text_obj_ + record_position_, command, num_bytes);
    record_position_ += num_bytes;
}

void _backend::_compiler::lineOfFunctionsView (nameTable* variables, tree_st index) {
    if (index) {
        if (strcmp (node_[index].name, "op") != 0)
            printf ("Error in operators of functions!\n");

        operatorsView       (variables, node_[index].left );
        lineOfFunctionsView (variables, node_[index].right);
    }
}

//      IN WORK
void _backend::_compiler::operatorsView (nameTable* variables, tree_st index) {
    if (index == 0)
        printf ("Error, \"op\" haven't left knot!\n");
    else {
        if (callFunctionsView (variables, index))
            return;
//        if (assignmentView (variables, index))
//            return;
//        if (operatorIfView (variables, index))
//            return;
//        if (operatorPutView (variables, index))
//            return;
//        if (operatorGetView (variables, index))
//            return;
//        if (operatorWhileView (variables, index))
//            return;
//        if (operatorReturnView (variables, index))
//            return;
    }
}

//      IN WORK
bool _backend::_compiler::callFunctionsView (nameTable* variables, tree_st index) {
    if ((node_[index].type == TYPE_OPERATOR && !strcmp (node_[index].name, "="))     // should be check second condition
    && (*node_[node_[index].right].name == '$')) {

        callFunctionsView (variables, node_[index].right);
        assignmentVariable_0 (variables, node_[index].left);
//        writeAssignmentVariable (variables, node_[index].left);
        return true;
    }

    if (*node_[index].name == '$') {
        int num_offset = writeArgumentFunction (variables, index);

        callFunction_0 (variables, index, num_offset);

//        if (strcmp (node_[node_[index].parent].name, "op") != 0)
//            writeNameInTextCode ("push cx\n");
        return true;
    }
    return false;
}

//  Write finction argiments when call and return number arguments
int _backend::_compiler::writeArgumentFunction (nameTable* variables, tree_st index) {
    int num_offset = 0;
    if (index) {
        tree_st end_index = index;
        while (node_[index].left != 0)  //  Go down to the far argument
            index = node_[index].left;

        while (index != end_index) {    //  Raise from bottom to top argument
            if (node_[node_[index].right].type == TYPE_VARIABLE)
                copyArgument_0 (variables, index);
            if (node_[node_[index].right].type == TYPE_OPERATOR) {
//                mathOperatorsView (variables, node_[index].right);

//                writeCopyArgument (variables, index, num_offset);
//                copyArgument_0 (variables, index);
            }

            index = node_[index].parent;
            num_offset++;
        }
//  In new version program it isn't use
//        writeOldValueBp (variables, num_offset);
    }
    return num_offset;
}
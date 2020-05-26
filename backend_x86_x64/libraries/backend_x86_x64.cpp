//
// Created by Texnar on 22.05.2020.
//

#include "backend_x86_x64.h"

#include "binCommands.h"


//---------------DEFINITION-OF-THE-STATIC-DATA-MEMBER---------------//
unsigned char*     _backend::_compiler::text_obj_;
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
    text_obj_ = (unsigned char*) calloc (DEFAULT_LENGTH_BIN_CODE, sizeof (unsigned char));
    if (!text_obj_) {
        printf ("Error in calloc in compiler!\n");
        abort ();
    }

    createAllFunctionLabel ();
    uploadDataFromTree ();
    coprocessorInitialization ();

    searchMainFunctionView (root_);
    mainLineView           (root_);

    connectionOfAdditionalFunctions ("add_func.obj");
    allFunctionAddressFilling ();
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
            callExit();
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
        return true;
    }
    return false;
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
    else
        return false;
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
        if (callFunctionsView  (variables, index))
            return;
        if (assignmentView     (variables, index))
            return;
        if (operatorIfView     (variables, index))
            return;
        if (operatorPutView    (variables, index))
            return;
        if (operatorGetView    (variables, index))
            return;
        if (operatorWhileView  (variables, index))
            return;
        if (operatorReturnView (variables, index))
            return;
    }
}

//      IN WORK
bool _backend::_compiler::callFunctionsView (nameTable* variables, tree_st index) {
    if ((node_[index].type == TYPE_OPERATOR && !strcmp (node_[index].name, "="))     // should be check second condition
    && (*node_[node_[index].right].name == '$')) {

        callFunctionsView (variables, node_[index].right);
        assignmentVariable_0 (variables, node_[index].left);
        return true;
    }

    if (*node_[index].name == '$') {
        int num_offset = writeArgumentFunction (variables, index);

        callFunction_0 (variables, index, num_offset);
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
                copyArgument_0 (variables, node_[index].right);
            if (node_[node_[index].right].type == TYPE_OPERATOR) {
                mathOperatorsView (variables, node_[index].right);
            }
            index = node_[index].parent;
            num_offset++;
        }
    }
    return num_offset;
}

void _backend::_compiler::writeInObjFile (const char* name_file) {
    FILE* file_obj = fopen (name_file, "wb");

    fwrite (text_obj_, sizeof (char), record_position_, file_obj);
    fclose (file_obj);
}

void _backend::_compiler::mathOperatorsView (nameTable* table, tree_st index) {
    if (!index)
        return;
    if (oneMathOperatorView (table, index, "+",   OPERATOR_ADD))
        return;
    if (oneMathOperatorView (table, index, "-",   OPERATOR_SUB))
        return;
    if (oneMathOperatorView (table, index, "*",   OPERATOR_MUL))
        return;
    if (oneMathOperatorView (table, index, "/",   OPERATOR_DIV))
        return;
    if (oneMathOperatorView (table, index, "^",   OPERATOR_POW))
        return;
    if (oneMathOperatorView (table, index, "sin", OPERATOR_SIN))
        return;
    if (oneMathOperatorView (table, index, "cos", OPERATOR_COS))
        return;

    if (*node_[index].name == '$') {
        callFunctionsView (table, index);
        return;
    }
    if (node_[index].type == TYPE_VARIABLE){
        writeValueVariable (table, index);
        return;
    }
    if (node_[index].type == TYPE_NUMBER){
        writeValueNumber (table, index);
        return;
    }
}

bool _backend::_compiler::oneMathOperatorView (nameTable* table, tree_st index, const char* name, value_t value) {
    if (!strcmp (node_[index].name, name)) {
        node_[index].type  = TYPE_OPERATOR;
        node_[index].value = value;
//        int now_priority = priorityFunction (index);
        mathOperatorsView (table, node_[index].left);
        mathOperatorsView (table, node_[index].right);

        //      Special case!      //
        if (value == OPERATOR_POW
        && (node_[node_[index].right].type  != TYPE_NUMBER
        ||  node_[node_[index].right].value != 50)) {     // 50 it is = 1/2 * 100
            printf ("Error, can be only pow = 0.5, only as a number!\n");
            abort ();
        }

        //-------------------------------Calling-functions-------------------------------//

        //  In this situation we can't use jmptable
        switch (value) {
            case OPERATOR_ADD:
                operatorAdd_0 ();
                break;
            case OPERATOR_SUB:
                operatorSub_0 ();
                break;
            case OPERATOR_MUL:
                operatorMul_0 ();
                break;
            case OPERATOR_DIV:
                operatorDiv_0 ();
                break;
            case OPERATOR_POW:
                operatorPow_0 ();
                break;
            case OPERATOR_SIN:
                operatorSin_0 ();
                break;
            case OPERATOR_COS:
                operatorCos_0 ();
                break;
            default:
                break;
        }

        return true;
    }
    return false;
}

/*
int _backend::_compiler::priorityFunction(tree_st index) {
    if (node_[index].type == 0)
        return 1;
    if (node_[index].type == TYPE_NUMBER ||
        node_[index].type == TYPE_VARIABLE)
           return 1;

    if (node_[index].type == TYPE_OPERATOR &&
        (node_[index].value == OPERATOR_ADD ||
         node_[index].value == OPERATOR_SUB))
        return 2;

    if (node_[index].type == TYPE_OPERATOR &&
        (node_[index].value == OPERATOR_MUL ||
         node_[index].value == OPERATOR_DIV))
        return 3;

    if (node_[index].type == TYPE_OPERATOR &&
        (node_[index].value == OPERATOR_POW))
        return 4;

    if (node_[index].type == TYPE_OPERATOR &&
        (node_[index].value == OPERATOR_LN
       ||node_[index].value == OPERATOR_SIN
       ||node_[index].value == OPERATOR_COS))
        return 5;
    printf("Error in priority function %d\n", node_[index].type);
}
*/

bool _backend::_compiler::assignmentView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "=") && *node_[node_[index].right].name != '$') {
        if (variables == nullptr) {
            printf ("YOU DON'T ADD GLOBAL VARIABLE!");
        }
        mathOperatorsView (variables, node_[index].right);

        assignmentVariable_0 (variables, node_[index].left);
        return true;
    }

    return false;
}

void _backend::_compiler::mainLineView (tree_st index) {
    if (!strcmp(node_[index].name, ";")) {
        secondLineView (node_[index].left);
        if (node_[index].right)
            mainLineView (node_[index].right);
    } else
        printf ("Error first line program");
}

void _backend::_compiler::secondLineView (tree_st index) {
    if (strcmp(node_[index].name, "$main") != 0)
        if (index != 0 && !functionView (index) && !assignmentView (0, index))
            printf ("Error, unknown branch from main line!\n");
}

bool _backend::_compiler::operatorIfView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "if")) {
        jmpblock jump[2] = {};

        compareView     (variables, node_[index].left, jump);
        allResultIfView (variables, node_[index].right, jump);
    }
    return false;
}

void _backend::_compiler::compareView (nameTable* variables, tree_st index, jmpblock* jump) {
    writeCopmareValues (variables, node_[index].left);
    writeCopmareValues (variables, node_[index].right);

    size_t byte_type = writeCompare ();

    if (!strcmp (node_[index].name, "=="))
        text_obj_[byte_type] = (byte) 0x85; //  jne
    if (!strcmp (node_[index].name, "!="))
        text_obj_[byte_type] = (byte) 0x84; //  je
    if (!strcmp (node_[index].name, "<"))
        text_obj_[byte_type] = (byte) 0x8D; //  jge
    if (!strcmp (node_[index].name, ">"))
        text_obj_[byte_type] = (byte) 0x8E; //  jle
    if (!strcmp (node_[index].name, ">="))
        text_obj_[byte_type] = (byte) 0x8C; //  jl
    if (!strcmp (node_[index].name, "<="))
        text_obj_[byte_type] = (byte) 0x8F; //  jg

    jump[0].from = record_position_ - 4;
}

void _backend::_compiler::writeCopmareValues (nameTable* variables, tree_st index) {
    mathOperatorsView (variables, index);
}

void _backend::_compiler::allResultIfView (nameTable* variables, tree_st index, jmpblock* jump) {
    if (!node_[index].left)
        printf ("Error, \"if\" haven't main result");

    lineOfFunctionsView (variables, node_[index].left);
    writeInObjText (com_jmp, sizeof (com_jmp));

    jump[1].from = record_position_ - 4;
    jump[0].to   = record_position_;
    uploadValueFromJmpBlock (&jump[0]);

    if (node_[index].right) {
        lineOfFunctionsView (variables, node_[index].right);
        jump[1].to = record_position_;
        uploadValueFromJmpBlock (&jump[1]);
    }
}

void _backend::_compiler::uploadValueFromJmpBlock (jmpblock* jump) {
    if (!jump->from || !jump->to) {
        printf ("There is not enough data in the jumping jmpblock!\n");
    }
    *(unsigned int*)(&text_obj_[jump->from]) = (unsigned int)(jump->to - jump->from - 4);
}

//  You can't have ret in main
bool _backend::_compiler::operatorReturnView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "ret")) {
        mathOperatorsView (variables, node_[index].left);
        if (node_[index].left != 0)
            writeInObjText (com_return_value, sizeof (com_return_value));

        writeInObjText (com_return_rbp, sizeof (com_return_rbp));
        writeInObjText (com_return, sizeof (com_return));
        return true;
    }
    return false;
}

bool _backend::_compiler::operatorWhileView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "while")) {
        jmpblock jump[2] = {};

        jump[1].to = record_position_;
        compareView (variables, node_[index].left, jump);

        //  Enter in code in while
        lineOfFunctionsView (variables, node_[index].right);

        writeInObjText (com_jmp, sizeof (com_jmp));
        jump[1].from = record_position_ - 4;
        jump[0].to   = record_position_;

        uploadValueFromJmpBlock (&jump[0]);
        uploadValueFromJmpBlock (&jump[1]);
    }
    return false;
}

bool _backend::_compiler::operatorPutView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "put")) {
        mathOperatorsView (variables, node_[index].left);
        callPutFunction_0 (variables, index);
        return true;
    }
    return false;
}

bool _backend::_compiler::operatorGetView (nameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "get")) {
        callGetFunction_0    (variables, index);
        assignmentVariable_0 (variables, node_[index].left);
        return true;
    }
    return false;
}

long itLength(FILE* file)
{
    assert (file != nullptr);

    fseek (file, 0, SEEK_END);
    long result = ftell (file);
    fseek (file, 0, SEEK_SET);

    return result;
}

void _backend::_compiler::connectionOfAdditionalFunctions (const char* name_func_file) {
    FILE* func_file = fopen (name_func_file, "rb");
    if (func_file == nullptr) {
        printf ("Not found file with functions!\n");
        abort();
    }
    long length_file = itLength (func_file) - 16;

    if (length_file + record_position_ >= DEFAULT_LENGTH_BIN_CODE) {
        printf ("Error in length of binary code, not enough space\n");
        abort ();
    }

    char file_hader[16] = {};
    fread (file_hader, 16, sizeof (char), func_file);

    if (file_hader[0] != 'M' || file_hader[1] != 'K') {
        printf ("Error, signature in file with function not true!\n");
        abort();
    }

    ntable_t index_get_func = functions.searchNameInTable ("get");
    if (index_get_func != -1)
        functions.var[index_get_func].position_object = ((ntable_t)*(int*)(&file_hader[4]) + (ntable_t)record_position_);

    ntable_t index_put_func = functions.searchNameInTable ("put");
    if (index_get_func != -1)
        functions.var[index_get_func].position_object = ((ntable_t)*(int*)(&file_hader[8]) + (ntable_t)record_position_);

    // Check sem length main program + add function

    fread (text_obj_ + record_position_, length_file, sizeof (char), func_file);
    record_position_ += length_file;
}

void _backend::_compiler::allFunctionAddressFilling () {
    ntable_t size = functions.size_;
    for (ntable_t i = 0; i < size; i++) {
        element* func = &functions.var[i];
        for (ntable_t j = 0; j < func->free_places; j++) {
            *(int*)(&text_obj_[func->position_depended[j]]) =
                    (int)(func->position_object - func->position_depended[j] - 4);
        }                           // We sub 4, because point for counter to jmp counted down from next command
    }
}
//
// Created by Texnar on 22.05.2020.
//

#include "backend_x86_64.h"
#include "bin_commands.h"


//---------------DEFINITION-OF-THE-STATIC-DATA-MEMBER---------------//
         bool      Backend::debug_              = false;
unsigned char*     Backend::text_obj_           = nullptr;
unsigned char*     Backend::text_exe_           = nullptr;
const    char*     Backend::name_file_to_write_ = "program.exe";
         Tree      Backend::tree_;
         NameTable Backend::functions;
size_t             Backend::record_position_    = 0;

//---------------DEFINITION-OF-THE-STATIC-DATA-MEMBER---------------//


void Backend::whatItIs (Node* node) {
    if (isOperator (node)) return;
    if (isNumber   (node)) return;
    if (isVariable (node)) return;
}

bool Backend::isOperator (Node* node)
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

bool Backend::isNumber   (Node* node) {
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

bool Backend::isVariable (Node* node) {
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

void Backend::treeColoring (FILE* file, Node* node) {
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

void Backend::compiler::compilingCode () {
    text_obj_ = (unsigned char*) calloc (DEFAULT_LENGTH_BIN_CODE, sizeof (unsigned char));
    if (!text_obj_) {
        throw "Failed to allocate the requested block of memory in compiler!";
    }

    createAllFunctionLabel ();
    uploadDataFromTree ();
    coprocessorInitialization ();

    searchMainFunctionView (root_);
    mainLineView           (root_);

    connectionOfAdditionalFunctions ("add_func.obj");
    allFunctionAddressFilling ();
}

void Backend::compiler::createAllFunctionLabel () {
    tree_.controlledExternalFunctionFromRoot (createOneFunctionLabel);
}

int Backend::compiler::createOneFunctionLabel (Node* node) {
    if (!strcmp (node->name, ";"))
        return tree_.BOTH_BRANCH;

    if (node->name[0] == '$') {
        if (functions.searchNameInTable (&(node->name[1])) == -1)
            functions.createNameInTable (&(node->name[1]));
        return tree_.NO_BRANCH;
    }

    return tree_.NO_BRANCH;
}

void Backend::compiler::uploadDataFromTree () {
    root_ = tree_.getRoot  ();
    node_ = tree_.getNodes ();
}

void Backend::compiler::searchMainFunctionView (tree_st index) {
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

bool Backend::compiler::functionView (tree_st index) {
    if (node_[index].name[0] == '$') {
        NameTable variable;

        searchAllVariablesInFunctionView (&variable, node_[index].left);
        variable.num_arguments_ = variable.size_;   // deteted +1
        searchAllVariablesInFunctionView (&variable, node_[index].right);

        startFunction_0 (&variable, index);

        lineOfFunctionsView (&variable, node_[index].right);
        return true;
    }
    return false;
}

void Backend::compiler::searchAllVariablesInFunctionView (NameTable* variables, tree_st index) {
    if (index) {
        if (node_[index].type == TYPE_VARIABLE &&
           !checkNameVariable (variables, node_[index].name))
            variables->createNameInTable (node_[index].name);
        searchAllVariablesInFunctionView (variables, node_[index].left );
        searchAllVariablesInFunctionView (variables, node_[index].right);
    }
}

bool Backend::compiler::checkNameVariable (NameTable* table, char* name) {
    if (table->searchNameInTable (name) != -1)
        return true;
    else
        return false;
}

void Backend::compiler::writeInObjText (const byte* command, size_t num_bytes) {
    if (text_obj_ == 0 || command == nullptr || num_bytes == 0) {
        throw "Error in arguments of 'writeInObjText'";
    }
    if (record_position_ + 10 >= DEFAULT_LENGTH_BIN_CODE) {
        throw "Error in length of binary code, not enough space";
    }

    memcpy (text_obj_ + record_position_, command, num_bytes);
    record_position_ += num_bytes;
}

void Backend::compiler::lineOfFunctionsView (NameTable* variables, tree_st index) {
    if (index) {
        if (strcmp (node_[index].name, "op") != 0)
            printf ("Error in operators of functions!\n");

        operatorsView       (variables, node_[index].left );
        lineOfFunctionsView (variables, node_[index].right);
    }
}

void Backend::compiler::operatorsView (NameTable* variables, tree_st index) {
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

bool Backend::compiler::callFunctionsView (NameTable* variables, tree_st index) {
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

//  Write function arguments when call and return number arguments
int Backend::compiler::writeArgumentFunction (NameTable* variables, tree_st index) {
    int num_offset = 0;
    if (index) {
        tree_st end_index = index;
        while (node_[index].left != 0)  //  Go down to the far argument
            index = node_[index].left;

        while (index != end_index) {    //  Raise from bottom to top argument
            if (node_[node_[index].right].type == TYPE_VARIABLE) {
                switch (optimization_) {
                    case 0:
                        copyArgument_0 (variables, node_[index].right);
                        break;
                    case 1:
                        copyArgument_1 (variables, node_[index].right);
                        break;
                    default:
                        throw "Unknown optimization!";
                        break;
                }
            }
            if (node_[node_[index].right].type == TYPE_OPERATOR) {
                mathOperatorsView (variables, node_[index].right);
            }
            index = node_[index].parent;
            num_offset++;
        }
    }
    return num_offset;
}

void Backend::compiler::writeInObjFile (const char* name_file) {
    FILE* file_obj = fopen (name_file, "wb");
    if (!file_obj) {
        throw "Can't open file in function dump!";
    }

    fwrite (text_obj_, sizeof (char), record_position_, file_obj);
    fclose (file_obj);
}

void Backend::compiler::mathOperatorsView (NameTable* table, tree_st index) {
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

bool Backend::compiler::oneMathOperatorView (NameTable* table, tree_st index, const char* name, value_t value) {
    if (!strcmp (node_[index].name, name)) {
        node_[index].type  = TYPE_OPERATOR;
        node_[index].value = value;

        mathOperatorsView (table, node_[index].left);
        mathOperatorsView (table, node_[index].right);

        //      Special case!      //
        if (value == OPERATOR_POW
        && (node_[node_[index].right].type  != TYPE_NUMBER
        ||  node_[node_[index].right].value != 50)) {     // 50 it is = 1/2 * 100
            throw "Error, can be only pow = 0.5, only as a number!";
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


bool Backend::compiler::assignmentView (NameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "=") && *node_[node_[index].right].name != '$') {
        if (variables == nullptr) {
            printf ("YOU DON'T ADD GLOBAL VARIABLE!");
        }
        mathOperatorsView    (variables, node_[index].right);
        assignmentVariable_0 (variables, node_[index].left);
        return true;
    }

    return false;
}

void Backend::compiler::mainLineView (tree_st index) {
    if (!strcmp(node_[index].name, ";")) {
        secondLineView (node_[index].left);
        if (node_[index].right)
            mainLineView (node_[index].right);
    } else
        printf ("Error first line program");
}

void Backend::compiler::secondLineView (tree_st index) {
    if (strcmp(node_[index].name, "$main") != 0)
        if (index != 0 && !functionView (index) && !assignmentView (0, index))
            printf ("Error, unknown branch from main line!\n");
}

bool Backend::compiler::operatorIfView (NameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "if")) {
        JmpBlock jump[2] = {};

        compareView     (variables, node_[index].left, jump);
        allResultIfView (variables, node_[index].right, jump);
    }
    return false;
}

void Backend::compiler::compareView (NameTable* variables, tree_st index, JmpBlock* jump) {
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

void Backend::compiler::writeCopmareValues (NameTable* variables, tree_st index) {
    mathOperatorsView (variables, index);
}

void Backend::compiler::allResultIfView (NameTable* variables, tree_st index, JmpBlock* jump) {
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

void Backend::compiler::uploadValueFromJmpBlock (JmpBlock* jump) {
    if (!jump->from || !jump->to) {
        printf ("There is not enough data in the jumping JmpBlock!\n");
    }
    *(unsigned int*)(&text_obj_[jump->from]) = (unsigned int)(jump->to - jump->from - 4);
}

//  You can't have ret in main
bool Backend::compiler::operatorReturnView (NameTable* variables, tree_st index) {
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

bool Backend::compiler::operatorWhileView (NameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "while")) {
        JmpBlock jump[2] = {};

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

bool Backend::compiler::operatorPutView (NameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "put")) {
        mathOperatorsView (variables, node_[index].left);
        callPutFunction_0 (index);
        return true;
    }
    return false;
}

bool Backend::compiler::operatorGetView (NameTable* variables, tree_st index) {
    if (!strcmp (node_[index].name, "get")) {
        callGetFunction_0    (index);
        assignmentVariable_0 (variables, node_[index].left);
        return true;
    }
    return false;
}

long itLength (FILE* file)
{
    assert (file != nullptr);

    fseek (file, 0, SEEK_END);
    long result = ftell (file);
    fseek (file, 0, SEEK_SET);

    return result;
}

void Backend::compiler::connectionOfAdditionalFunctions (const char* name_func_file) {
    FILE* func_file = fopen (name_func_file, "rb");
    if (func_file == nullptr) {
        throw "Not found file with functions (for developer)!";
    }
    long length_file = itLength (func_file) - 16;

    // Check sum length main program + add function
    if (length_file + record_position_ >= DEFAULT_LENGTH_BIN_CODE) {
        throw "Error in length of binary code, not enough space (for developer)";
    }

    char file_hader[16] = {};
    fread (file_hader, 16, sizeof (char), func_file);

    if (file_hader[0] != 'M' || file_hader[1] != 'K') {
        throw "Signature in file with function not true, wrong file transferred (for developer)!";
    }

    entryInTheTableServiceFunctions (file_hader);

    fread (text_obj_ + record_position_, length_file, sizeof (char), func_file);
    record_position_ += length_file;
}

void Backend::compiler::entryInTheTableServiceFunctions (char header_additional_file[]) {
    ntable_t index_get_func = functions.searchNameInTable ("get");
    if (index_get_func != -1)
        functions.var_[index_get_func].position_object =
                ((ntable_t)*(int*)(&header_additional_file[4]) + (ntable_t)record_position_);

    ntable_t index_put_func = functions.searchNameInTable ("put");
    if (index_put_func != -1)
        functions.var_[index_put_func].position_object =
                ((ntable_t)*(int*)(&header_additional_file[8]) + (ntable_t)record_position_);


    ntable_t index_exit_func = functions.searchNameInTable ("exit");
    if (index_exit_func != -1)
        functions.var_[index_exit_func].position_object =
                ((ntable_t)*(int*)(&header_additional_file[12]) + (ntable_t)record_position_);
}

void Backend::compiler::allFunctionAddressFilling () {
    ntable_t size = functions.size_;
    for (ntable_t i = 0; i < size; i++) {
        Element* func = &functions.var_[i];
        for (ntable_t j = 0; j < func->free_places; j++) {
            *(int*)(&text_obj_[func->position_depended[j]]) =
                    (int)(func->position_object - func->position_depended[j] - 4);
        }                           // We sub 4, because point for counter to jmp counted down from next command
    }
}

//-----------------------------------------FIRST-LINKING---(IN-WORK)-----------------------------------------//
void Backend::linker::firstLinking () {
    text_exe_ = (unsigned char*) calloc (DEFAULT_LENGTH_BIN_CODE, sizeof (unsigned char));
    if (!text_exe_) {
        throw "Failed to allocate the requested block of memory! (for developer)";
    }

    createDosHeader ();
    createPeHeader  ();
    createSectionHeader ();

}

void Backend::linker::createDosHeader () {
    _IMAGE_DOS_HEADER dos_header = {};
    dos_header.e_magic = 0x5A4D;
    dos_header.e_lfanew = 0x40;     // <-- From this plasce start PE header

    writeInExeText ((unsigned char*) &dos_header, sizeof (dos_header));
}

void Backend::linker::writeInExeText (const unsigned char* command, size_t num_bytes) {
    if (text_exe_ == 0 || command == nullptr || num_bytes == 0) {
        throw "Error in arguments of 'writeInObjText'";
    }
    if (record_position_exe_ + 10 >= DEFAULT_LENGTH_BIN_CODE) {
        throw "Error in length of binary code, not enough space";
    }

    memcpy (text_exe_ + record_position_exe_, command, num_bytes);
    record_position_exe_ += num_bytes;
}

void Backend::linker::createPeHeader () {
    _IMAGE_NT_HEADERS pe_header = {};
    pe_header.Signature = 0x00004550;
    pe_header.FileHeader.Machine              = IMAGE_FILE_MACHINE_AMD64  ;
    pe_header.FileHeader.NumberOfSections     = 2;
    pe_header.FileHeader.SizeOfOptionalHeader = IMAGE_SIZEOF_NT_OPTIONAL64_HEADER;
    pe_header.FileHeader.Characteristics      = 0x102;
                                      // 0x100 - that program for architecture win32
                                      // 0x002 - This indicates that the image file is valid and can be run

    pe_header.OptionalHeader.Magic                 = IMAGE_NT_OPTIONAL_HDR64_MAGIC ; // x64
    pe_header.OptionalHeader.AddressOfEntryPoint   = 0x1000;
    pe_header.OptionalHeader.ImageBase             = 0x400000;
    pe_header.OptionalHeader.SectionAlignment      = 0x1000;
    pe_header.OptionalHeader.FileAlignment         = 0x200;
    pe_header.OptionalHeader.MajorSubsystemVersion = 4;
    pe_header.OptionalHeader.SizeOfImage           = 0x4000;
    pe_header.OptionalHeader.SizeOfHeaders         = 0x200;
    pe_header.OptionalHeader.Subsystem             = 2;
    pe_header.OptionalHeader.NumberOfRvaAndSizes   = 16;

    pe_header.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = 0x2000;  // Import Directory

    writeInExeText ((unsigned char*)&pe_header, sizeof (pe_header));    // Write our struct in file

}

void Backend::linker::createSectionHeader () {
    _IMAGE_SECTION_HEADER section_header[2] = {};

    //--------------------SECTION-.TEXT--------------------//
    strcpy ((char*)(section_header[0].Name), ".text");
    section_header[0].Misc.VirtualSize = 0x1000;
    section_header[0].VirtualAddress   = 0x1000;
    section_header[0].SizeOfRawData    = 0x200;    // size physical cost memory
    section_header[0].PointerToRawData = 0x200;    // physical offset
    section_header[0].Characteristics  = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;
    //--------------------SECTION-.TEXT--------------------//

    //--------------------SECTION-.RDATA--------------------//
    strcpy ((char*)(section_header[1].Name), ".rdata");
    section_header[1].Misc.VirtualSize = 0x1000;
    section_header[1].VirtualAddress   = 0x2000;
    section_header[1].SizeOfRawData    = 0x1000;   // size physical cost memory
    section_header[1].PointerToRawData = 0x200;    // physical offset
    section_header[1].Characteristics  = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;
    //--------------------SECTION-.RDATA--------------------//

    writeInExeText ((unsigned char*)&section_header[0],
                    2 * sizeof (section_header[0]));    // Write our struct in file

}

void Backend::linker::writeExeInFile(const char* name_file) {
    FILE* file_exe = fopen (name_file, "wb");

    fwrite (text_exe_, sizeof (char), record_position_exe_, file_exe);
    fclose (file_exe);
}

//-----------------------------------------FIRST-LINKING---(IN-WORK)-----------------------------------------//


void Backend::linker::secondLinking (const char* name_file) {
    assert (name_file != nullptr);
    FILE* empty_program = fopen ("empty.exe", "rb");
    assert (empty_program != nullptr);
    size_t length_file = itLength (empty_program);
    unsigned char* empty_text = (unsigned  char*) calloc (length_file, sizeof (char));

    fread (empty_text, sizeof (char), length_file, empty_program);

    memcpy (empty_text + 512, text_obj_, record_position_);
    memcpy (empty_text + record_position_ - 12 + 512, com_jmp, sizeof (com_jmp));
    memcpy (empty_text + record_position_ - 20 + 512, com_jmp, sizeof (com_jmp));
    memcpy (empty_text + record_position_ - 28 + 512, com_jmp, sizeof (com_jmp));
    memcpy (empty_text + record_position_ - 36 + 512, com_jmp, sizeof (com_jmp));

//    *(empty_text + record_position_ - 28 + 512 - 3) = 90;   // To help debugger undestand command
//    *(empty_text + record_position_ - 20 + 512 - 3) = 90;   //
//    *(empty_text + record_position_ - 12 + 512 - 3) = 90;   //

    *(int*)(empty_text + record_position_ + 512 - 36 + 1) = (8192 - (record_position_ - 36) - 5);  // Read ConsoleA
    *(int*)(empty_text + record_position_ + 512 - 28 + 1) = (8200 - (record_position_ - 28) - 5);  // GetHandle
    *(int*)(empty_text + record_position_ + 512 - 20 + 1) = (8208 - (record_position_ - 20) - 5);  // WriteConsoleA
    *(int*)(empty_text + record_position_ + 512 - 12 + 1) = (8216 - (record_position_ - 12) - 5);  // Exit

    fclose (empty_program);

    FILE* done_program = fopen (name_file, "wb");
    assert (done_program != 0);
    fwrite (empty_text, sizeof (char), length_file, done_program);
    fclose (done_program);
    free   (empty_text);
}

void Backend::freeMemory() {
    if (text_obj_ != nullptr) {
        free (text_obj_);
    }
    if (text_exe_ != nullptr) {
        free (text_exe_);
    }
}

void Backend::compiler::checkArguments (int num_arguments, char ** strings) {
    assert (num_arguments > 0);
    assert (strings != 0);

    for (int j = 1; j < num_arguments; j++) {
        char* temp_op = strstr (strings[j], "-O");
        if (temp_op) {
            int optimization = 0;
            sscanf (temp_op + 2, "%d", &optimization);
            if (optimization == 0 || optimization == 1) {
                optimization_ = optimization;
            } else {
                throw "Invalid optimization!";
            }
        }

        if (!strcmp (strings[j], "-o")) {
            name_file_to_write_ = strings[j+1];
            j++;
        }

        if (strcmp (strings[j], "--d")) {
            debug_ = true;
        }
    }
}

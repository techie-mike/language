//
// Created by Texnar on 22.05.2020.
//

#ifndef BACKEND_X86_X64_BACKEND_X86_64_H
#define BACKEND_X86_X64_BACKEND_X86_64_H

#include "windows.h"
#include <winnt.h>

#include "base/my_tree.h"
#include "name_table.h"


class Backend {
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
    static          Tree    tree_;
    static          size_t  record_position_;
    static    const char*   name_file_to_write_;
    static          bool    debug_;

    //==============================COMPILER===============================//
    class compiler {  // class for function and variable for binary compiler
    public:
        void compilingCode ();
        void writeInObjFile (const char* name_file);
        void checkArguments (int num_arguments, char ** strings);

    private:
        int          optimization_    {};
        tree_st      root_            {};
        Node*        node_            {};

        inline void createAllFunctionLabel ();
        static int  createOneFunctionLabel (Node* node);

        void uploadDataFromTree ();

        //-------------------------WRITE-OBJ-TEXT-----------------------------
        void searchMainFunctionView (tree_st index);
        struct JmpBlock {
            size_t from;
            size_t to;
        };

        bool functionView (tree_st index);

        void searchAllVariablesInFunctionView (NameTable* variables, tree_st index);
        bool checkNameVariable                (NameTable* table, char* name);

        void coprocessorInitialization ();

        //.................................................................//
        void operatorsView          (NameTable* variables, tree_st index);

        bool callFunctionsView      (NameTable* variables, tree_st index);
        int  writeArgumentFunction  (NameTable* variables, tree_st index);
        void lineOfFunctionsView    (NameTable* variables, tree_st index);

        void mathOperatorsView      (NameTable* table, tree_st index);
        bool oneMathOperatorView    (NameTable* table, tree_st index, const char* name,
                                     value_t value);

        void writeValueVariable     (NameTable* table, tree_st index);
        void writeValueNumber       (tree_st index);

        bool assignmentView         (NameTable* variables, tree_st index);

        void mainLineView           (tree_st index);
        void secondLineView         (tree_st index);

        bool operatorIfView         (NameTable* variables, tree_st index);
        void compareView            (NameTable* variables, tree_st index, JmpBlock* jump);
        void writeCopmareValues     (NameTable* variables, tree_st index);

        void allResultIfView        (NameTable* variables, tree_st index, JmpBlock* jump);
        bool operatorReturnView     (NameTable* variables, tree_st index);

        bool operatorWhileView      (NameTable* variables, tree_st index);
        bool operatorPutView        (NameTable* variables, tree_st index);
        bool operatorGetView        (NameTable* variables, tree_st index);


        void uploadValueFromJmpBlock (JmpBlock* jump);

        //---------------------INCLUDE-COMMAND-FUNCTION--------------------//
        #include "decl_com_functions.inc"
        //---------------------INCLUDE-COMMAND-FUNCTION--------------------//

        void writeInObjText (const unsigned char* command, size_t num_bytes);
        void connectionOfAdditionalFunctions (const char* name_func_file);
        void entryInTheTableServiceFunctions (char header_additional_file[]);
        void allFunctionAddressFilling ();
    } compiler_;

    static unsigned char* text_obj_;
    static NameTable functions;
    //==============================COMPILER===============================//

    //==============================LINKER=================================//
    static unsigned char* text_exe_;
    class linker {
    public:
        void firstLinking ();
        void writeExeInFile(const char* name_file);
        void secondLinking (const char* name_file);
    private:
        void createDosHeader ();
        void createPeHeader  ();

        size_t record_position_exe_;


        void writeInExeText (const unsigned char* command, size_t num_bytes);
        void createSectionHeader ();
    } linker_;

    //==============================LINKER=================================//


    //------------------------------FUNCTION-------------------------------//
    void freeMemory();
    static void whatItIs     (Node* node);     // check what type this Node
    static void treeColoring (FILE* file, Node* node);

private:
    static bool isOperator (Node* node);
    static bool isNumber   (Node* node);
    static bool isVariable (Node* node);
};

#endif //BACKEND_X86_X64_BACKEND_X86_64_H

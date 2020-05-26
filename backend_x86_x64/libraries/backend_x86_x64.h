//
// Created by Texnar on 22.05.2020.
//

#ifndef BACKEND_X86_X64_BACKEND_X86_X64_H
#define BACKEND_X86_X64_BACKEND_X86_X64_H

#include "windows.h"
#include <winnt.h>

#include "MyTree.h"
#include "NameTable.h"
//#include <cstring>



struct backend {
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
    static size_t       record_position_;

    //==============================COMPILER===============================//
    struct compiler {  // struct for function and variable for binary compiler
        void compilingCode ();
        void writeInObjFile (const char* name_file);

    private:
        int          optimization_    {};
        tree_st      root_            {};
        Node*        node_            {};

        inline void createAllFunctionLabel ();
        static int  createOneFunctionLabel (Node* node);

        void uploadDataFromTree ();

        //-------------------------WRITE-OBJ-TEXT-----------------------------
        void searchMainFunctionView (tree_st index);
        struct jmpblock {
            size_t from;
            size_t to;
        };

        bool functionView           (tree_st index);

        void searchAllVariablesInFunctionView (nameTable* variables, tree_st index);
        bool checkNameVariable                (nameTable* table, char* name);

        void coprocessorInitialization ();

        //.................................................................//
        void operatorsView          (nameTable* variables, tree_st index);

        bool callFunctionsView      (nameTable* variables, tree_st index);
        int  writeArgumentFunction  (nameTable* variables, tree_st index);
        void lineOfFunctionsView    (nameTable* variables, tree_st index);

        void mathOperatorsView      (nameTable* table, tree_st index);
        bool oneMathOperatorView    (nameTable* table, tree_st index, const char* name, value_t value);
//        int  priorityFunction       (tree_st index);

        void writeValueVariable     (nameTable* table, tree_st index);
        void writeValueNumber       (nameTable* variables, tree_st index);

        bool assignmentView         (nameTable* variables, tree_st index);

        void mainLineView           (tree_st index);
        void secondLineView         (tree_st index);

        bool operatorIfView         (nameTable* variables, tree_st index);
        void compareView            (nameTable* variables, tree_st index, jmpblock* jump);
        void writeCopmareValues     (nameTable* variables, tree_st index);

        void allResultIfView        (nameTable* variables, tree_st index, jmpblock* jump);
        bool operatorReturnView     (nameTable* variables, tree_st index);

        bool operatorWhileView      (nameTable* variables, tree_st index);
        bool operatorPutView        (nameTable* variables, tree_st index);
        bool operatorGetView        (nameTable* variables, tree_st index);



        void uploadValueFromJmpBlock (jmpblock* jump);

        //---------------------INCLUDE-COMMAND-FUNCTION--------------------//
        #include "declComFunctions.h"

        //---------------------INCLUDE-COMMAND-FUNCTION--------------------//
        void writeInObjText (const unsigned char* command, size_t num_bytes);
        void connectionOfAdditionalFunctions (const char* name_func_file);
        void allFunctionAddressFilling ();
    } compiler;

    static unsigned char* text_obj_;
    static nameTable functions;
    //==============================COMPILER===============================//

    //==============================LINKER=================================//
    static unsigned char* text_exe_;
    struct linker {
        void linking ();
        void writeExeInFile(const char* name_file);
        void secondLinking (const char* name_file);
    private:
        void createDosHeader ();
        void createPeHeader  ();

        size_t record_position_exe_;


        void writeInExeText (const unsigned char* command, size_t num_bytes);
        void createSectionHeader ();
    } linker;

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


#endif //BACKEND_X86_X64_BACKEND_X86_X64_H

//
// Created by texnar on 11/11/2019.
//

#ifndef AKINATOR_MYtree_H
#define AKINATOR_MYtree_H

#include "../../frontend/libraries/com_language.h"
#include "my_stack.h"


//#include "../token.h"
#include <ctype.h>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//#include "connect_asm.h"



/*enum {
    TYPE_OPERATOR = 1,
    TYPE_NUMBER   = 2,
    TYPE_VARIABLE = 3

    #define CMD(name, use_name, code) ,name_##use_name
    #include "../com_language.h"
    #undef  CMD
};*/


const char TYPE_OPERATOR = 1, TYPE_NUMBER = 2, TYPE_VARIABLE = 3,
           TYPE_FUNCTION = 4;

//extern Tokens;
enum {OPERATOR_ADD = 1, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV, OPERATOR_POW,
        OPERATOR_SIN, OPERATOR_COS, OPERATOR_LN};

enum {VARIABLE_X = 1, VARIABLE_Y, VARIABLE_Z, VARIABLE_T};


typedef int size_tree_t;
typedef double value_t;
//static size_tree_t DEFAULT_LENGTH = 100;



struct elem {
    char *name_;
    size_tree_t type_;
    value_t value_;

    size_tree_t left_;
    size_tree_t right_;
    size_tree_t parent_;
};

struct Tree {

    Tree (size_tree_t DEFAULT_LENGTH = 50, size_tree_t DEFAULT_LENGTH_NAMES = 200);
    ~Tree ();
    void dump ();
    void latex (Tree* origin,const char* name);

    bool readTreeFromFile (int num_arg, char *poin_arg[]);
    bool writeTreeInFile (const char name_file[]);

    void fullDifferential (Tree *diff_tree);

    Tree(const Tree& that) = delete;

    void writeConvertCode (const char name_file[]);

    void diffFunc ();
    int optimisationUnusedMembers (bool main_func = false);

    int optimisationOfConstants ();

    size_tree_t searchDeriv (size_tree_t index);

//    bool readTreeFromTokens (Tokens *tokens);

private:
//    nameTable label;  //may be I will be have problem with them
    int free_label_;

    elem *one_element;
    char *all_names;
    size_tree_t size_;
    size_tree_t length_;
    size_tree_t size_names_;
    size_tree_t length_names_;
    size_tree_t root_;
    size_tree_t free_;



    struct variables {
        bool x, y, z, t;
    } var_found = {};

    //----frontend-1 functions----//
    char* write_text_;
    int num_write_text_;
    int num_tabs_;
    bool last_str_have_n_;

/*
    void writeNameInTextCode (char* name);
    void writeNameInTextCode (const char* name);
    void writeNameInTextCode (int name_num);

    void assignmentWriteInTextCode ();

    void treeView (size_tree_t index);
    void mainLineView (size_tree_t index);
    void secondLineView (size_tree_t index);
*/

   /* bool assignmentView (nameTable* table, size_tree_t index);
    bool functionsView (size_tree_t index);
//    bool callFunctionsView (size_tree_t index, bool is_call = false);
    void argumentsOfFunctionsView (size_tree_t index, bool first = true);

    void lineOfFunctionsView (nameTable* table, size_tree_t index);
    void operatorsView (nameTable* table, size_tree_t index);
*/
/*

    //---------operators----------//
    bool callFunctionsView (nameTable* table, size_tree_t index);

    bool operatorIfView (nameTable* table, size_tree_t index);
    void allResultIfView (nameTable* table, size_tree_t index, int using_label);
    void resultIfView (nameTable* table, size_tree_t index);
    void compareView (nameTable* table, size_tree_t index, int using_label);

    bool oneMathOperatorView (nameTable* table, size_tree_t index, const char* name, size_tree_t type,
                        value_t value, const char* real_name, int last_priority = 0);
    void mathOperatorsView (nameTable* table, size_tree_t index, int last_priority = 0);
    void operatorDerivView (nameTable* table, size_tree_t index);

    bool operatorPutView (nameTable* table, size_tree_t index);
    bool operatorGetView (nameTable* table, size_tree_t index);

    bool operatorWhileView (nameTable* table, size_tree_t index);
    bool operatorReturnView (nameTable* table, size_tree_t index);

    void writeCopyArgument (nameTable* table, size_tree_t index, int num_offset);
    //---------operators----------//

*/


/*


    //---------backend-----------//
    void searchMainFunctionView (size_tree_t index);
    void createAllFunctionLabel (size_tree_t index);
    void searchAllVariablesInFuncView (nameTable* table, size_tree_t index);

//    void searchAllVariablesInArgFuncView (nameTable* table, size_tree_t index);

    bool checkNameVariable (nameTable* table, char* name);

    int writeArgumentFunction (nameTable* table, size_tree_t index);

    void copyArguments (nameTable* table, size_tree_t index, int num_offset);

    void writeOldValueBp (nameTable* table, int num_offset);
    void createNewValueBp (nameTable* table, int num_offset);
    void returnOldValueBp (nameTable* table);
    void readOldValueBp (int num_offset);

    void writeAssignmentVariable (nameTable* table, size_tree_t index);
    void writeValueVariable (nameTable* table, size_tree_t index);
    void writeValueNumber (nameTable* table, size_tree_t index);
    void writeValueNumberAndVariable (nameTable* table, size_tree_t index);

    void writeLabel (int using_label);
    void writeStartFunction (size_tree_t index);
    void writeEndFunction (size_tree_t index);
    //---------backend-----------//

*/


    //----frontend-1 functions----//

#include "diffur.h"

    //----recursive descent-----//

//    #define OPERS(func, name) size_tree_t func ();
//    #include "../func_operators.h"
//    #undef OPERS

    //----recursive descent----//

    bool itIsCmd (const char* name_command); // conformityCommand

    void deleteLastBracket (char *text);

    bool haveQuotes (char** read_now);

    void copyTree (Tree& copy);




    void autoLengthIncrease(int factor = 2);
    void autoLengthNamesIncrease(int factor = 2);
    size_tree_t createNewNode (Tree *diff_tree, size_tree_t type, value_t value);
    size_tree_t copyNode (Tree *diff_tree, size_tree_t index);
    size_tree_t createNumber(Tree* tree, value_t value);

    size_tree_t copyBranch (Tree *diff_tree, size_tree_t index);


    void searchParents (Stack_t* stack, size_tree_t index);

    void fillingPoisonousValues();

    size_tree_t seeBranch (char* name, size_tree_t index);
    size_tree_t checkName (char* name);


    static void writeNameInTextFromTree (char* text, char* name);
    void writeTree (char* text, size_tree_t index);
    void loadingTree (char text[]);
    void readTextTree (char* read_now);

    void skipSymbols (char** read_now);
    size_tree_t readNewObject (char** read_now);
    void readName (char** read_now, char name[]);
    size_tree_t createNewObject (char name[], size_tree_t left, size_tree_t right, size_tree_t parent = 0);

    void writeFulTreeInFile (char* text, const char *name_file);

};

#endif //AKINATOR_MYtree_H

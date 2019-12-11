//
// Created by texnar on 11/11/2019.
//

#ifndef AKINATOR_MYtree_H
#define AKINATOR_MYtree_H

#include "../com_language.h"
#include "my_stack.h"

#include "../token.h"
#include <ctype.h>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>




/*enum {
    TYPE_OPERATOR = 1,
    TYPE_NUMBER   = 2,
    TYPE_VARIABLE = 3

    #define CMD(name, use_name, code) ,name_##use_name
    #include "../com_language.h"
    #undef  CMD
};*/


const char TYPE_OPERATOR = 1, TYPE_NUMBER = 2, TYPE_VARIABLE = 3;

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

    bool readTreeFromFile (const char name_file[]);
    bool writeTreeInFile (const char name_file[]);

    void fullDifferential (Tree *diff_tree);

    Tree(const Tree& that) = delete;

    bool readTreeFromTokens (Tokens *tokens);

private:
    elem *one_element;
    char *all_names;
    size_tree_t size_;
    size_tree_t length_;
    size_tree_t size_names_;
    size_tree_t length_names_;
    size_tree_t root_;
    size_tree_t free_;

    Tokens* tokens_;

    token_names_t point_read_;

    struct variables {
        bool x, y, z, t;
    } var_found = {};

#include "diffur.h"

    //----recursive descent----//
    size_tree_t getG ();
    size_tree_t getBranch ();
    size_tree_t getCreate ();

    size_tree_t getFunc ();
    size_tree_t getE ();

  /*  size_tree_t getE ();
    size_tree_t getT ();
    size_tree_t getO ();

    size_tree_t getP ();

    size_tree_t getN ();
*/
    size_tree_t getId ();

    //----recursive descent----//

    bool itIsCmd (const char* name_command); // conformityCommand

    void writeErrorSyntax ();

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

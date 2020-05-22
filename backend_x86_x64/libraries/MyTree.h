//
// Created by Texnar on 21.05.2020.
//

#ifndef MYTREE_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "my_stack.h"


typedef int    tree_st;     // tree size type
typedef double value_t;

struct Node {
    char   *name   ;
    tree_st type   ;
    value_t value  ;

    tree_st left   ;
    tree_st right  ;
    tree_st parent ;
};

struct Tree {
private:
    Node        *node_          ;
    char        *all_names_     ;

    tree_st     size_           ;
    tree_st     length_         ;
    tree_st     size_names_     ;
    tree_st     length_names_   ;
    tree_st     root_           ;
    tree_st     free_           ;

public:
    Tree (tree_st DEFAULT_LENGTH       = 50 ,
          tree_st DEFAULT_LENGTH_NAMES = 200);
    ~Tree ();

    void dump ();

    void readTreeFromFile (char*      name_file  );
    bool writeTreeInFile  (const char name_file[]);
    void writeConvertCode (const char name_file[]);

    Tree (const Tree& that) = delete;


    void externalFunction (tree_st index, void (*func)(Node& node));
    //  Function for launch external functions, not from Tree.
    //  func parametr REFERENCE!!!

private:
//----------------------------FUNCTIONS----------------------------//
    static char* readTextFromFile (char* name_file);

    //--------------LOADING-TREE--------------//
    void loadingTree  (char  text[]  );    // load tree from text in this struct
    void readTextTree (char* read_now);

    //-------------------------------------------
    tree_st readNewObject          (char** read_now);
    tree_st readNewObjectBranch    (char** read_now);

    inline static bool haveQuotes  (char** read_now);
    static void        readName    (char** read_now, char name[]);
    //-------------------------------------------

    tree_st createNewObject (char    name[]     ,
                             tree_st left       ,
                             tree_st right      ,
                             tree_st parent = 0);



    //--------------SERVICE-DYNAMIC--------------//
    void autoLengthIncrease      (int factor = 2);
    void autoLengthNamesIncrease (int factor = 2);
    void fillingPoisonousValues  ();
    //--------------SERVICE-DYNAMIC--------------//
};

//-------------STATIC-FUNCTION-------------//

//-------------STATIC-FUNCTION-------------//

#define MYTREE_H

#endif //BACKEND_X86_X64_MYTREE_H

//
// Created by Texnar on 21.05.2020.
//

#ifndef MYTREE_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

#define LEVEL_VERIFIC 0
#include "my_stack.h"


typedef int    tree_st;     // tree size type
typedef long long value_t;

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
    tree_st     size_names_     ;
    tree_st     root_           ;
    tree_st     free_           ;
    tree_st     length_         ;
    tree_st     length_names_   ;

public:
    enum { NO_BRANCH = 0, ONLY_LEFT_BRANCH, ONLY_RIGHT_BRANCH, BOTH_BRANCH };

    Tree (tree_st DEFAULT_LENGTH       = 50 ,
          tree_st DEFAULT_LENGTH_NAMES = 200);
    ~Tree ();
    Tree (const Tree& that) = delete;

    void readTreeFromFile (char* name_file  );
    void dump (void (*colorFunction)(FILE* file, Node* node)); // Pass the function to change the colors of the blocks

    void fullVisit (void (*func)(Node* node));

//    void externalFunction (tree_st index, void (*func)(Node& node));
            //  Function for launch external functions, not from Tree.
            //  "func" parametr REFERENCE!!!

    void controlledExternalFunction (tree_st index, int (*func)(Node* node));
        //                             !!!IMPOTANT!!!                                    //
        //  Function "controlledExternalFunction" goes into the branches (left & right)  //
        //  depending on the return value of the "func":                                 //
        //       return  NO_BRANCH           don't go anywhere                           //
        //       return  ONLY_LEFT_BRANCH    go into ONLY left  branch                   //
        //       return  ONLY_RIGHT_BRANCH   go into ONLY right branch                   //
        //       return  BOTH_BRANCH         go into both branch                         //
        //-------------------------------------------------------------------------------//

    void controlledExternalFunctionFromRoot (int (*func)(Node* node));
        //  Based on "controlledExternalFunction", but this function
        //  always start from "root" index

private:
//----------------------------FUNCTIONS----------------------------//
    static char* readTextFromFile (char* name_file);
    static long  itLength         (FILE* file);

    void visit (tree_st index, void (*func)(Node* node));


    //----------------LOADING-TREE----------------//
    void loadingTree  (char  text[]  );    // load tree from text in this struct
    void readTextTree (char* read_now);

    //----------------------------------------------
    tree_st readNewObject          (char** read_now);
    tree_st readNewObjectBranch    (char** read_now);

    inline static bool haveQuotes  (char** read_now);
    static void        readName    (char** read_now, char name[]);
    //----------------------------------------------

    tree_st createNewObject (char    name[]     ,
                             tree_st left       ,
                             tree_st right      ,
                             tree_st parent = 0);
    //----------------LOADING-TREE----------------//


    //--------------SERVICE-DYNAMIC--------------//
    void autoLengthIncrease      (int factor = 2);
    void autoLengthNamesIncrease (int factor = 2);
    void fillingPoisonousValues  ();
    //--------------SERVICE-DYNAMIC--------------//
};


#define MYTREE_H

#endif //BACKEND_X86_X64_MYTREE_H

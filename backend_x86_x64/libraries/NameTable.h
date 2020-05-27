//
// Created by texnar on 16/12/2019.
//

#ifndef BACKEND_CONNECT_ASM_H
#define BACKEND_CONNECT_ASM_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <assert.h>

typedef long long ntable_t;

// Not optimal use for variable!!!
struct element {
    static const int DEFAULT_LENGTH_OF_ARRAY_POINTER = 50;
    char*  name;
    ntable_t  position_object;
    ntable_t  position_depended[DEFAULT_LENGTH_OF_ARRAY_POINTER];  // temporary solution
    int       free_places;

    bool   state;            // local or global

    void loadNewDependedPosition (ntable_t new_position);
};


struct nameTable {
    element* var;

    ntable_t num_arguments_;      // it include 1 num for "bp" !!!
    ntable_t size_;

    nameTable ();
   ~nameTable ();

    ntable_t searchNameInTable (const char* name);
    void     createNameInTable (      char* name);
    void     createNameInTable (const char* name);

    nameTable& operator=(const nameTable&) = delete;
    nameTable (const nameTable&) = delete;
private:
    char*    all_names_;
    ntable_t size_names_;

    ntable_t length_;
    ntable_t length_names_;
    ntable_t free_;

    const int DEFAULT_LENGTH       = 5 ;
    const int DEFAULT_LENGTH_NAMES = 40;

    void autoLengthIncrease      (int factor = 2);
    void autoLengthNamesIncrease (int factor = 2);

    void fillingPoisonousValues ();
};

#endif //BACKEND_CONNECT_ASM_H

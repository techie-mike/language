//
// Created by texnar on 16/12/2019.
//

#ifndef NAME_TABLE_H
#define NAME_TABLE_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <assert.h>

typedef long long ntable_t;

struct Element {
    static const int DEFAULT_LENGTH_OF_ARRAY_POINTER = 50;
    char* name;
    ntable_t  position_object;
    ntable_t  position_depended[DEFAULT_LENGTH_OF_ARRAY_POINTER];  // temporary solution
    int       free_places;

    bool state; // local or global
    void loadNewDependedPosition (ntable_t new_position);
};


class NameTable {
public:
    Element* var_;

    ntable_t num_arguments_;      // it include 1 num for "bp" !!!
    ntable_t size_;

    NameTable ();
   ~NameTable ();

    ntable_t searchNameInTable (const char* name);
    void     createNameInTable (      char* name);
    void     createNameInTable (const char* name);

    NameTable& operator=(const NameTable&) = delete;
    NameTable (const NameTable&) = delete;
private:
    char*    all_names_;
    ntable_t size_names_;

    ntable_t length_;
    ntable_t length_names_;
    ntable_t free_;

    const int DEFAULT_LENGTH_       = 5 ;
    const int DEFAULT_LENGTH_NAMES_ = 40;

    void autoLengthIncrease      (int factor = 2);
    void autoLengthNamesIncrease (int factor = 2);

    void fillingPoisonousValues ();
};

#endif //NAME_TABLE_H

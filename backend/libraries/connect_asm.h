//
// Created by texnar on 16/12/2019.
//

#ifndef BACKEND_CONNECT_ASM_H
#define BACKEND_CONNECT_ASM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int ntable_t;

struct variable {
    char* name;
    bool  state;            // local or global
};

struct nameTable {
    variable* var;

    ntable_t num_arg_;      // it include 1 num for "bp" !!!

    nameTable ();
   ~nameTable ();

    ntable_t searchNameInTable (      char* name);
    void     createNameInTable (      char* name);
    void     createNameInTable (const char* name);

	nameTable& operator = (const nameTable& other) = delete;
    ntable_t size_;
private:
    char*    all_names_;
    ntable_t size_names_;

    ntable_t length_;
    ntable_t length_names_;
    ntable_t free_;

    const int DEFAULT_LENGTH       = 5 ;
    const int DEFAULT_LENGTH_NAMES = 20;

    void autoLengthIncrease      (int factor = 2);
    void autoLengthNamesIncrease (int factor = 2);

    void fillingPoisonousValues ();
};

#endif //BACKEND_CONNECT_ASM_H

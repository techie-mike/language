//
// Created by Андрей on 23.05.2020.
//

#ifndef BACKEND_X86_X64_DECLCOMFUNCTIONS_H
#define BACKEND_X86_X64_DECLCOMFUNCTIONS_H

void startFunction_0      (nameTable* variables, tree_st index);

void callFunction_0       (nameTable* variables, tree_st index, int num_parameters);
void assignmentVariable_0 (nameTable* variables, tree_st index);
void copyArgument_0       (nameTable* variables, tree_st index);

ntable_t loadElementIndex (nameTable* variables, tree_st index);
size_t   writeCompare();

void callGetFunction_0      (nameTable* variables, tree_st index);
void callPutFunction_0      (nameTable* variables, tree_st index);


//------------------MATH-OPERATOR-FUNCTIONS------------------//
    void operatorAdd_0 ();
    void operatorSub_0 ();
    void operatorMul_0 ();
    void operatorDiv_0 ();
    void operatorPow_0 ();
    void operatorSin_0 ();
    void operatorCos_0 ();

#endif //BACKEND_X86_X64_DECLCOMFUNCTIONS_H

//
// Created by Texnar on 23.05.2020.
//

#include "backend_x86_x64.h"
#include "binCommands.h"

typedef unsigned long long type_proc;

#define LOADCOMMAND(name_variable, name_command) \
    byte name_variable[sizeof (name_command)] = {};\
    memcpy (name_variable, name_command, sizeof (name_command));
//#include "declComFunctions.h"

// DONE
void _backend::_compiler::startFunction_0 (nameTable* variables, tree_st index) {
    byte command[sizeof (com_start_function_0)] = {};   // Copy commnad from data base
    memcpy (command, com_start_function_0,
             sizeof (com_start_function_0));

    ntable_t offset_local_variables = 8 * (variables->size_ - variables->num_arguments_);
    ntable_t block_function = functions.searchNameInTable (node_[index].name + 1);

    functions.var[block_function].position_object = record_position_;

    *(type_proc*)(&command[5]) = (type_proc) offset_local_variables;
    writeInObjText (command, sizeof (command));
}

// DONE
void _backend::_compiler::callFunction_0 (nameTable* variables, tree_st index, int num_parameters) {
    ntable_t block_function = functions.searchNameInTable (node_[index].name + 1);
    element* call_func      = &functions.var[block_function];

    LOADCOMMAND (command, com_call_function_0);
    *(type_proc*)(&command[14]) = (type_proc) (8 * num_parameters);

    type_proc position_call_in_text = record_position_ + 2;
    call_func->loadNewDependedPosition ((long long) position_call_in_text);

    writeInObjText (command, sizeof (command));
}

// DONE
void _backend::_compiler::assignmentVariable_0 (nameTable* variables, tree_st index) {
    LOADCOMMAND (command, com_assignment_variable_0);
    ntable_t index_in_ram = loadElementIndex (variables, index);

    *(type_proc*)(&command[4]) = (type_proc) index_in_ram;
    writeInObjText (command, sizeof (command));
}

// DONE
void _backend::_compiler::copyArgument_0 (nameTable* variables, tree_st index) {
    LOADCOMMAND (command, com_push_arguments_0);

    ntable_t index_in_ram = loadElementIndex (variables, index);


    //          |----|----|----||old rbp|ret|----|----|     <-- stack growth
    //          \            / ^             \       /
    //           \          / rbp             \     /
    //            \________/                   \___/
    //        local_variables               num_arguments_

    *(type_proc*)(&command[3]) = (type_proc) index_in_ram;

    writeInObjText (command, sizeof (command));
}

// DONE
ntable_t _backend::_compiler::loadElementIndex (nameTable* variables, tree_st index) {
    ntable_t index_variable = variables->searchNameInTable (node_[index].name);
    if (index_variable == -1) {
        printf ("Error in loadElement index, search not exist variable: %s\n", node_[index].name);
        assert (index_variable != -1);
    }
    ntable_t num_variable = 8 * (variables->searchNameInTable (node_[index].name) - variables->num_arguments_ + 1);
    if (num_variable <= 0)
        num_variable -= 16;
    return num_variable;
}

void _backend::_compiler::operatorAdd_0 () {
    writeInObjText (com_operator_add_0, sizeof (com_operator_add_0));
}

void _backend::_compiler::operatorSub_0 () {
    writeInObjText (com_operator_sub, sizeof (com_operator_sub));
}

void _backend::_compiler::operatorMul_0 () {
    writeInObjText (com_operator_mul_0, sizeof (com_operator_mul_0));
}
void _backend::_compiler::operatorDiv_0 () {
    writeInObjText (com_operator_div_0, sizeof (com_operator_div_0));
}

void _backend::_compiler::operatorPow_0 () {
    writeInObjText (com_operator_pow, sizeof (com_operator_pow));
}

void _backend::_compiler::operatorSin_0 () {
    writeInObjText (com_operator_sin, sizeof (com_operator_sin));
}

void _backend::_compiler::operatorCos_0 () {
    writeInObjText (com_operator_cos, sizeof (com_operator_cos));
}

void _backend::_compiler::writeValueVariable (nameTable* variables, tree_st index) {
    copyArgument_0 (variables, index);
}

void _backend::_compiler::writeValueNumber (nameTable* variables, tree_st index) {
    LOADCOMMAND(command, com_push_number);
    *(type_proc*)(&command[2]) = (type_proc) node_[index].value;
}

void _backend::_compiler::coprocessorInitialization () {
    writeInObjText (com_init_coproc, sizeof (com_init_coproc));
}

//  Returns byte position where jump type changes
size_t _backend::_compiler::writeCompare() {
    writeInObjText (com_compare_and_jmp, sizeof (com_compare_and_jmp));
    return record_position_ - 5;
}
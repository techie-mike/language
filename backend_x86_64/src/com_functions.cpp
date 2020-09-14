//
// Created by Texnar on 23.05.2020.
//

#include "backend_x86_64.h"
#include "bin_commands.h"

typedef unsigned long long type_proc;
//=====================IMPORTANT INFORMATION=====================
//     Index after name function (for example, func_0) means
//     level of optimization:
//     ..._0      zero level
//     ..._1      first level
//===============================================================
#define LOADCOMMAND(name_variable, name_command) \
    byte name_variable[sizeof (name_command)] = {};\
    memcpy (name_variable, name_command, sizeof (name_command));


void Backend::compiler::startFunction_0 (NameTable* variables, tree_st index) {
    byte command[sizeof (com_start_function_0)] = {};   // Copy commnad from data base
    memcpy (command, com_start_function_0,
             sizeof (com_start_function_0));

    ntable_t offset_local_variables = 8 * (variables->size_ - variables->num_arguments_);
    ntable_t block_function = functions.searchNameInTable (node_[index].name + 1);

    functions.var_[block_function].position_object = record_position_;

    *(type_proc*)(&command[5]) = (type_proc) offset_local_variables;
    writeInObjText (command, sizeof (command));
}


void Backend::compiler::callFunction_0 (tree_st index, int num_parameters) {
    ntable_t block_function = functions.searchNameInTable (node_[index].name + 1);
    if (block_function == -1) {
        throw "No function implementation! (for developer)";
    }
    Element* call_func = &functions.var_[block_function];

    LOADCOMMAND (command, com_call_function_0);
    *(type_proc*)(&command[7]) = (type_proc) (8 * num_parameters);

    type_proc position_call_in_text = record_position_ + 1;
    call_func->loadNewDependedPosition ((long long) position_call_in_text);

    writeInObjText (command, sizeof (command));
}


void Backend::compiler::assignmentVariable_0 (NameTable* variables, tree_st index) {
    LOADCOMMAND (command, com_assignment_variable_0);
    ntable_t index_in_ram = loadElementIndex (variables, index);

    *(type_proc*)(&command[4]) = (type_proc) index_in_ram;
    writeInObjText (command, sizeof (command));
}


void Backend::compiler::copyArgument_0 (NameTable* variables, tree_st index) {
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

void Backend::compiler::copyArgument_1 (NameTable* variables, tree_st index) {
    LOADCOMMAND (command, com_push_arguments_1);

    ntable_t index_in_ram = loadElementIndex (variables, index);

    //          |----|----|----||old rbp|ret|----|----|     <-- stack growth
    //          \            / ^             \       /
    //           \          / rbp             \     /
    //            \________/                   \___/
    //        local_variables               num_arguments_

    *(type_proc*)(&command[2]) = - (type_proc) index_in_ram;
    writeInObjText (command, sizeof (command));
}


ntable_t Backend::compiler::loadElementIndex (NameTable* variables, tree_st index) {
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

void Backend::compiler::operatorAdd_0 () {
    writeInObjText (com_operator_add_0, sizeof (com_operator_add_0));
}

void Backend::compiler::operatorSub_0 () {
    writeInObjText (com_operator_sub, sizeof (com_operator_sub));
}

void Backend::compiler::operatorMul_0 () {
    writeInObjText (com_operator_mul_0, sizeof (com_operator_mul_0));
}
void Backend::compiler::operatorDiv_0 () {
    writeInObjText (com_operator_div_0, sizeof (com_operator_div_0));
}

void Backend::compiler::operatorPow_0 () {
    writeInObjText (com_operator_pow, sizeof (com_operator_pow));
}

void Backend::compiler::operatorSin_0 () {
    writeInObjText (com_operator_sin, sizeof (com_operator_sin));
}

void Backend::compiler::operatorCos_0 () {
    writeInObjText (com_operator_cos, sizeof (com_operator_cos));
}

void Backend::compiler::writeValueVariable (NameTable* variables, tree_st index) {
    switch (optimization_) {
        case 0:
            copyArgument_0 (variables, index);
            break;
        case 1:
            copyArgument_1 (variables, index);
            break;
        default:
            printf ("Unknown optimization!\n");
            break;
    }
}

void Backend::compiler::writeValueNumber (tree_st index) {
    LOADCOMMAND(command, com_push_number);
    *(type_proc*)(&command[2]) = (type_proc) node_[index].value;
    writeInObjText (command, sizeof (command));
}

void Backend::compiler::coprocessorInitialization () {
    writeInObjText (com_init_coproc, sizeof (com_init_coproc));
}

//  Returns byte position where jump type changes
size_t Backend::compiler::writeCompare() {
    writeInObjText (com_compare_and_jmp, sizeof (com_compare_and_jmp));
    return record_position_ - 5;
}

void Backend::compiler::callGetFunction_0 (tree_st index) {
    writeInObjText (com_call_get_0, sizeof (com_call_get_0));

    if (functions.searchNameInTable (node_[index].name) == -1)
        functions.createNameInTable (node_[index].name);
    ntable_t index_function = functions.searchNameInTable (node_[index].name);
    functions.var_[index_function].loadNewDependedPosition ((long long) (record_position_ - 5));
}

void Backend::compiler::callPutFunction_0 (tree_st index) {
    writeInObjText (com_call_put_0, sizeof (com_call_put_0));
    if (functions.searchNameInTable (node_[index].name) == -1)
        functions.createNameInTable (node_[index].name);
    ntable_t index_function = functions.searchNameInTable (node_[index].name);
    functions.var_[index_function].loadNewDependedPosition ((long long) (record_position_ - 4));
}

void Backend::compiler::callExit () {
    writeInObjText (com_call_exit, sizeof (com_call_exit));

    functions.createNameInTable ("exit");
    ntable_t index_function = functions.searchNameInTable ("exit");
    functions.var_[index_function].loadNewDependedPosition ((long long) (record_position_ - 4));
}
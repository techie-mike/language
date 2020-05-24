//
// Created by Texnar on 23.05.2020.
//

#include "backend_x86_x64.h"
#include "binCommands.h"

void _backend::_compiler::start_function_0 (nameTable* table, tree_st index, long long num_local_variable) {
    byte command[10] = {};
    memcpy (command, com_start_function_0,
             sizeof (com_start_function_0));

    ntable_t block_function = table->searchNameInTable (node_[index].name + 1);
    table->var[block_function].position_object = record_position_;

    *(unsigned int*)(&command[6]) = 8 * (unsigned int) num_local_variable;
    writeInObjText (command, sizeof (command));
}

void _backend::_compiler::call_function_0 (nameTable* table, tree_st index) {
    ntable_t block_function = table->searchNameInTable (node_[index].name + 1);
    element* call_func = &table->var[block_function];
//    call_func->position_depended[call_func->free_places];
    call_func->free_places++;

}

void _backend::_compiler::assignment_variable_0 (nameTable* table, tree_st index) {
    
}
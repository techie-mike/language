//
// Created by Texnar on 23.05.2020.
//

#ifndef BACKEND_X86_X64_BINCOMMANDS_H
#define BACKEND_X86_X64_BINCOMMANDS_H

typedef unsigned char byte;

//---------------------COMMANDS-O0---------------------//
//  Don't worry, commands O0 speshial VERY SLOWLY!!!

const byte com_start_function_0[] =
        {
        0x55,                   // push rbp
        0x54,                   // push rsp
        0x5D,                   // pop  rbp
        0x48, 0xB8,             // mov  rax, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00, //

        0x48, 0x29, 0xC4        // sub  rsp, rax
        };

const byte com_copy_arguments_0[] =
        {
        0x55,                   // push rbp
        0x48, 0xBB,             // mov  rbx, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00, //

        0x58,                   // pop  rax
        0x48, 0x29, 0xD8,       // sub  rax, rbx
        0xFF, 0x30              // push QWORD ptr [rax]
        };

const byte com_call_function_0[] =
        {
        0x48, 0xB8,             // mov  rax, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00,

        0xFF, 0xD0,             // call rax
        0x48, 0xBB,             // mov  rbx, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00,

        0x48, 0x01, 0xDC,       // add  rsp, rbx
        0x50                    // push rax
        };

//const byte loading_element_address_in_rax[] =
//        {
//        0x55,                   // push rbp
//        0x48, 0xBB,             // mov  rbp, imm64
//
//        0x00, 0x00, 0x00, 0x00, // imm64
//        0x00, 0x00, 0x00, 0x00,
//
//        0x48, 0x29, 0xD8,       // sub  rax, rbx
//        0x48, 0x83, 0xC0, 0x10  // add  rax, 0x10 (or will be 'nop' x4)
//        };

const byte assignment_variable_0[] =
        {
        0x55,                   // push rbp
        0x58,                   // pop rax
        0x48, 0xBB,             // mov rbx, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00,

        0x48, 0x29, 0xD8,       // sub rax, rbx
        0x8F, 0x00              // pop QWORD ptr [rax]
        };

#endif //BACKEND_X86_X64_BINCOMMANDS_H

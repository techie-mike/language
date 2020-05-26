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

        0x48, 0x29, 0xC4       // sub  rsp, rax
        };

const byte com_init_coproc[] =
        {
        0x9B, 0xDB, 0xE3        // finit
        };

const byte com_push_arguments_0[] =
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
//        0x48, 0xB8,             // mov  rax, imm64
//
//        0x00, 0x00, 0x00, 0x00, // imm64
//        0x00, 0x00, 0x00, 0x00,
//
//        0xFF, 0xD0,             // call rax

        0xE8, 0x00, 0x00, 0x00, 0x00, // call 0x0

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

const byte com_assignment_variable_0[] =
        {
        0x55,                   // push rbp
        0x58,                   // pop rax
        0x48, 0xBB,             // mov rbx, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00,

        0x48, 0x29, 0xD8,       // sub rax, rbx
        0x8F, 0x00              // pop QWORD ptr [rax]
        };

const byte com_operator_add_0[] =
        {
        0x5B,               // pop rbx
        0x58,               // pop rax
        0x48, 0x01, 0xD8,   // add rax, rbx
        0x50                // push rax
        };

const byte com_operator_sub[] =
        {
        0x5B,               // pop rbx
        0x58,               // pop rax
        0x48, 0x29, 0xD8,   // sub rax, rbx
        0x50                // push rax
        };

const byte com_operator_mul_0[] =
        {
        0x5B,                   // pop  rbx
        0x58,                   // push rax
        0x48, 0xF7, 0xEB,       // imul  rbx

        0x48, 0xC7, 0xC3,       // mov  rbx, 0x64
        0x64, 0x00, 0x00, 0x00,

        0x48, 0xF7, 0xFB,       // idiv rbx
        0x50                    // push rax
        };
const byte com_operator_div_0[] =
        {
        0x5B,                   // pop rbx
        0x58,                   // pop rax

        0x48, 0xC7, 0xC2,       // mov rdx, 0x64
        0x64, 0x00, 0x00, 0x00,

        0x48, 0xF7, 0xEA,       // imul rdx
        0x48, 0xF7, 0xFB,       // idiv rbx
        0x50                    // push rax
        };

const byte com_operator_pow[] =
        {
        0x5B,                   // pop rbx (then not use)
        0x58,                   // pop rax

        0x48, 0xC7, 0xC3,       // mov rbx, 0xa
        0x0A, 0x00, 0x00, 0x00,

        0x48, 0xF7, 0xEB,       // imul  rbx
        0x50,                   // push rax
        0x48, 0x89, 0xE3,       // mov  rbx, rsp
        0xDB, 0x03,             // fild DWORD PTR [rbx]
        0xD9, 0xFA,             // fsqrt
        0xDB, 0x1B,             // fistp DWORD PTR [rbx]
        0x48, 0x89, 0xEC        // mov  rsp, rbp
        };

const byte com_operator_sin[] =
        {
        0x48, 0x89, 0xE3,   // mov rbp, rsp
        0xDB, 0x03,         // fild DWORD PTR [rbx]
        0xD9, 0xFE,         // fsin
        0xDB, 0x1B          // fistp DWORD PTR [rbx]
        };

const byte com_operator_cos[] =
        {
        0x48, 0x89, 0xE3,   // mov rbp, rsp
        0xDB, 0x03,         // fild DWORD PTR [rbx]
        0xD9, 0xFF,         // fcos
        0xDB, 0x1B          // fistp DWORD PTR [rbx]
        };

const byte com_push_number[] =
        {
        0x48, 0xBB,             // mov rbx, imm64

        0x00, 0x00, 0x00, 0x00, // imm64
        0x00, 0x00, 0x00, 0x00,

        0x53                    // push rbx
        };

const byte com_compare_and_jmp[] =
        {
        0x5B,                   // pop rbx
        0x58,                   // pop rax
        0x48, 0x39, 0xD8,       // cmp rax, rbx

        0x0F, 0x84,             // je 0
        0x00, 0x00, 0x00, 0x00  // relative address for jmp
        };

const byte com_jmp[] =
        {
        0xE9,                   // jmp 0
        0x00, 0x00, 0x00, 0x00  // relative address for jmp
        };

const byte com_return[] =
        {
        0xC3    // ret
        };
const byte com_return_value[] =
        {
        0x58    // pop rax
        };

const byte com_call_get_0[] =
        {
        0xE8, 0x00, 0x00, 0x00, 0x00, // call 0x0 (relative address for jmp)

        0x50                    // push 0x50
        };

const byte com_call_put_0[] =
        {
        0x51,                           // push rcx
        0xE8, 0x00, 0x00, 0x00, 0x00    // call 0x0 (relative address)
        };

const byte com_call_exit[] =
        {
        0xE8, 0x00, 0x00, 0x00, 0x00    // call 0x0 (relative address)
        };

const byte com_return_rbp[] =
        {
        0x55,   // push rbp
        0x5C,   // pop rsp
        0x5D    // pop rbp
        };
#endif //BACKEND_X86_X64_BINCOMMANDS_H

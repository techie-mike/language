//
// Created by Texnar on 23.05.2020.
//

#ifndef BACKEND_X86_X64_BINCOMMANDS_H
#define BACKEND_X86_X64_BINCOMMANDS_H

typedef unsigned char byte;

//---------------------COMMANDS-O0---------------------//

const byte com_start_function_0[] =
        { 0x55,                     // push rbp
          0x54,                     // push rsp
          0x5D,                     // pop  rbp
          0x48, 0x81, 0xEC,         // sub rsp
          0x00, 0x00, 0x00, 0x00    // number for sub
        };



#endif //BACKEND_X86_X64_BINCOMMANDS_H

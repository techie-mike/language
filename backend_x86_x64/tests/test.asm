                                                ; Console Message, 64 bit. V1.03
NULL              EQU 0                         ; Constants
STD_OUTPUT_HANDLE EQU -11

extern GetStdHandle                             ; Import external symbols
extern WriteFile                                ; Windows API functions, not decorated
extern ExitProcess
extern WriteConsoleA

global Start                                    ; Export symbols. The entry point

section .data                                   ; Initialized data segment
 Message        db "Console Message 64", 0Dh, 0Ah
 MessageLength  EQU $-Message                   ; Address of this line ($) - address of Message


section .text                                   ; Code segment
Start:

 sub   RSP, 0                                   ; Align the stack to a multiple of 16 bytes
 sub   RSP, 32                                  ; 32 bytes of shadow space
 mov   ECX, STD_OUTPUT_HANDLE
 call  GetStdHandle
 mov   rcx, RAX
 add   RSP, 32                                  ; Remove the 32 bytes
 sub   RSP, 32 + 8                          ; Shadow space + 5th parameter + align stack
                                                ; to a multiple of 16 bytes
 lea   RDX, [REL Message]                       ; 2nd parameter
 mov   R8, 10                        ; 3rd parameter
 mov   R9, 0                        ; 4th parameter
 mov   qword [RSP + 4 * 8], NULL                ; 5th parameter
 call  WriteConsoleA                                ; Output can be redirect to a file using >
 add   RSP, 40                                  ; Remove the 48 bytes

 xor   ECX, ECX
 call  ExitProcess
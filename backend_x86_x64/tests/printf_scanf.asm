; Written on nasm + ld + WinAPI

;	./nasm printf_scanf.asm -f win64 -l printf_scanf.lst
;	./ld64 printf_scanf.obj user32.dll kernel32.dll


extern GetStdHandle
extern WriteConsoleA
extern ReadConsoleA
extern ExitProcess

global Start

;STD_OUTPUT_HANDLE	equ	-11

section .data
text	db	"Hello, world!\n"


section .text
Start:        

;----------------SCANF----------------
		and esp, ~0xf
		
		sub rsp, 32				;	new 32 bytes of shadow space
		mov rcx, -10
		call GetStdHandle
		mov rcx, rax
		add rsp, 32
		
		sub rsp, 8
		mov rdi, rsp
		sub rsp, 8
		mov rsi, rsp
		
		sub rsp, 32 + 8 + 8
		mov dword[rsp+32], 0
		mov r9,rsi
		mov r8, 8
		mov rdx, rdi
		call ReadConsoleA
		add rsp, 48
		add rsp, 8
		
;----------------SCANF----------------
		
;---------------PRINTF----------------	
		sub rsp, 32
		mov rcx, -11
		call GetStdHandle
		mov rcx, rax		
		add rsp, 32				;	end of shadow space

		sub rsp, 32 + 8			;	new 32 bytes of shadow space 
		mov dword [rsp + 32], 0
		mov r9, rsi
		mov r8, 10
		mov rdx, rdi
		call WriteConsoleA
		
		add rsp, 48
		
;----------------EXIT-----------------
		xor rcx, rcx
		call ExitProcess
	
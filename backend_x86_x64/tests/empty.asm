; Written on nasm + ld + WinAPI

;	.\nasm printf_scanf.asm -f win64 -l printf_scanf.lst
;	.\ld64 printf_scanf.obj user32.dll kernel32.dll


extern GetStdHandle
extern WriteConsoleA
extern ReadConsoleA
extern ExitProcess

global Start

section .text
	call GetStdHandle
	times 8192-5 nop
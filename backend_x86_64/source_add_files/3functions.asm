; Written on nasm + ld + WinAPI

;	./nasm 3functions.asm -f win64 -l 3functions.lst
;	./ld64 3functions.obj user32.dll kernel32.dll

extern GetStdHandle
extern WriteConsoleA
extern ReadConsoleA
extern ExitProcess

global Start

section .text
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
scan:    
;---------------------------------------
;	Read in console number with 
;	fixed accuracy 
;	Ret: RAX	input value
;---------------SCAN--------------------
	
		push rbp
		push rsp
		pop  rbp
		
		
		and rsp, ~0xf
		
		sub rsp, 32
		mov rcx, -10
		call GetStdHandle
		
		mov rcx, rax
		sub rsp, 32 + 16
		
		mov qword [rsp + 32], 0 	; fifth arguments in stack
		lea r9, [rsp + 48]
		mov r8, 18
		
		lea rdx, [rsp + 40]
		call ReadConsoleA
		
;------------------------------
		cld
		
		xor rax, rax
		xor r9,  r9
		mov r8,  10
		
		mov rbx, 0x2E
		mov rcx, 18
		lea rsi, [rsp + 40]
		
	;------------------------; Check minus
		xor r10, r10
		lodsb
		cmp al, 45
		jne skip_min
		inc rsi
		inc r10
	skip_min:
		dec rsi
	;------------------------
	while:
		lodsb
		cmp al, bl
		jne skip
		mov rcx, 2		; for numbers after point 1000.00
		jmp while
	skip:	
		xchg rax, r9
		
		imul rax, r8	; rdx *= 10
		xchg rax, r9
		
		sub al, 48
		add r9, rax
		loop while
		
		cmp r10, 1
		jne not_minus_scan
		neg r9
	not_minus_scan:	
		
		mov rax, r9		; return value in rcx if form  ddddd.dd
		
		push rbp
		pop  rsp
		pop  rbp
		
		ret
;---------------SCAN--------------------
	nop
	nop
	nop
	nop
	nop
;---------------------------------------
;	Write in console number with 
;	fixed accuracy 
;---------------------------------------
;	Entry:	RCX	value
;---------------PRINT-------------------
Start:	
		push rbp
		push rsp
		pop  rbp
		
		and rsp, ~0xf
		
		lea rdi, [rsp-1]
		xor r10, r10
		
		std
		
		mov al, 10		; add '\n' at the end of string
		stosb
		inc r10
		
		mov rsi, rsp
		sub rsp, 32 + 16
		mov rax, rcx

		xor r12, r12
		
		mov r8, 100
		mov rcx, 18
		
		mov rbx, 10
		
		xor r11, r11
		cmp rax, 0
		jge looop
		neg rax
		inc r11
	;------------------
	looop:					; write num as a string in reverse order
		cmp r10, 3
		jne continue
		;============
		mov r8, rax
		mov rax, 0x2E
		stosb
		inc r10	
		mov rax, r8
		;^^^^^^^^^^^^
	continue:
		cqo
		idiv rbx
		xchg rax, rdx
		add al, 48
		stosb
		xchg rax, rdx
		
		inc r10			; it is counter for num in number
		cmp rax, 0
		je exit
		
		loop looop
	;-------------------	
	exit:
		cmp r10, 3
		jg not_empty_start
		;+++++++++++++++++
		mov al, 46
		stosb
		inc r10
		
		mov al, 48
		stosb
		inc r10
		;+++++++++++++++++
	not_empty_start:	
		cmp r11, 1
		jne not_minus
		mov al, 45
		stosb
		inc r10
	not_minus:
		cld
		
		sub rsp, 32 + 16
		mov rcx, -11
		call GetStdHandle
		
		mov rdx, rsi
		sub rdx, r10
		mov r8,  r10
		mov rcx, rax
		mov qword [rsp + 32], 0
		lea r9, [rsp + 32 + 8]
		
		call WriteConsoleA
		
		push rbp
		pop  rsp
		pop  rbp
		
		ret
		
;---------------PRINT-------------------		
		
	nop
	nop
	nop
	nop
	nop
		
;	Exit from program
;-----------------EXIT------------------
	Exit:
		xor rcx, rcx
		call ExitProcess
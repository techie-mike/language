quad: frontend_quad middle_end backend assembler
	../cpu/./processor ../logs/CODE.nasm

test: frontend_test middle_end backend assembler
	../cpu/./processor ../logs/CODE.nasm

test_d: frontend_test_d middle_end_d  backend_d_ assembler
	../cpu/./processor ../logs/CODE.nasm

deriv: frontend_deriv middle_end backend assembler
	../cpu/./processor ../logs/CODE.nasm

deriv_d: frontend_deriv_d middle_end_d backend_d assembler
	../cpu/./processor ../logs/CODE.nasm

frontend_test:
	./frontend ../programs/test.gop

frontend_deriv:
	./frontend ../programs/deriv.gop

frontend_deriv_d:
	./frontend ../programs/deriv.gop --d


quad_d: frontend_quad_d middle_end_d backend_d assembler
	../cpu/./processor ../logs/./CODE.nasm
frontend_quad_d: ../programs/quad_work.gop
	./frontend ../programs/quad_work.gop --d

frontend_quad:  ../programs/quad_work.gop
	./frontend ../programs/quad_work.gop


middle_end: ../logs/tree.txt
	./middle_end ../logs/tree.txt


middle_end_d: ../logs/tree.txt
	./middle_end ../logs/tree.txt --d


backend: ../logs/tree.txt
	./backend ../logs/tree.txt

backend_d: ../logs/tree.txt
	./backend ../logs/tree.txt --d




assembler: ../logs/code.n
	../cpu/./Assembler ../logs/code.n

frontend_back: ../logs/tree.txt
	./frontend_back ../logs/tree.txt

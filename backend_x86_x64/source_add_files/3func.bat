.\nasm 3functions.asm -f win64 -l 3functions.lst

.\ld64 3functions.obj user32.dll kernel32.dll -o 3functions.exe

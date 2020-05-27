echo GOP compiler  (C) Mikhail Kaskov

.\frontend ..\programs\fact_win1251.gop --d
.\backend_x86_x64 ..\logs\tree.txt -O1 -o ..\logs\fact.exe
..\logs\fact.exe


::-------------------AND-FOR-MY-CPU-------------------
::.\backend ..\logs\tree.txt
::..\cpu\.\Assembler ..\logs\code.n
::..\cpu\.\processor ..\logs\CODE.nasm
::-------------------AND-FOR-MY-CPU-------------------
echo GOP compiler  (C) Mikhail Kaskov

.\frontend ..\programs\fact_time.gop --d
.\backend_x86_x64 ..\logs\tree.txt -O0 -o ..\logs\fact_time.exe
::Measure-Command { ..\logs\fact_time.exe }



.\backend ..\logs\tree.txt
..\cpu\.\Assembler ..\logs\code.n
..\cpu\.\processor ..\logs\CODE.nasm

::Measure-Command { ..\cpu\.\processor ..\logs\CODE.nasm }
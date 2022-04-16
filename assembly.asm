extern printf, scanf, exit
section .data
		b2d: dd 0.0
		int_fmt: db "%d", 10, 0
		real_fmt: db "%lf", 10, 0
		zero: dw 0
		flttmp: dq 0.0
section .text
global main
main:
                fld	dword [b2d]	            ; need to convert 32-bit to 64-bit 
	            fstp	qword [flttmp]      ; floating load makes 80-bit,
	                                        ; store as 64-bit 
                push dword [flttmp+4]       
                push dword [flttmp]         
                push dword real_fmt 
                call printf 
                add ESP,12                 
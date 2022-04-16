<<<<<<< HEAD
=======
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
<<<<<<< HEAD
				ENTER 4, 0
				;reserve space for the input/output params of fn, later flush this space
		                    mov word [b2d], 5 
                    		                    mov word [b3d], 4 
                    		                    mov EDX, EBP 
                    sub EDX, 0 
                    mov word [EDX], 6 
                                    mov EDX, EBP
                sub EDX, 0     ; make EDX to point at location of variable on the stack
                push word [zero] ;
                push word [EDX]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                MOV ECX, 0
        MOV EDX,1
                push word [zero] ;
                push word [b2d]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
=======
                fld	dword [b2d]	            ; need to convert 32-bit to 64-bit 
	            fstp	qword [flttmp]      ; floating load makes 80-bit,
	                                        ; store as 64-bit 
                push dword [flttmp+4]       
                push dword [flttmp]         
                push dword real_fmt 
>>>>>>> d44fe13e343dec20007f1496d374cc1bd8f645bc
                call printf 
                add ESP,12                 
>>>>>>> e4ec893d8547ab62e2dcab5b9ac38e0c0f3a725b

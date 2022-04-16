extern printf, scanf, exit
section .data
		b2d: dw 0
		b3d: dw 0
		int_fmt: db "%d", 10, 0
		real_fmt: db "%lf", 10, 0
		zero: dw 0
		flttmp: dq 0.0
section .text
global main
main:
				ENTER 4, 0
				;reserve space for the input/output params of fn, later flush this space
		                    mov word [b2d], 5 
                    		                    mov word [b3d], 4 
                    		                    mov EDX, EBX 
                    sub EDX, 0 
                    mov word [EDX], 6 
                    MOV ECX, 0
        MOV EDX,1
                push word [zero] ;
                push word [b2d]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                                push word [zero] ;
                push word [b3d]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                				;Deallocate space given to I/O variables on stack
				add ESP, 4
				LEAVE

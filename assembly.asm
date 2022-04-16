extern printf, scanf, exit
section .data
		int_fmt: db "%d", 10, 0
		real_fmt: db "%lf", 10, 0
		zero: dw 0
		flttmp: dq 0.0
section .text
global main
main:
				ENTER 8, 0
				;reserve space for the input/output params of fn, later flush this space
		                    mov EDX, EBP 
                    sub EDX, 2 
                    mov word [EDX], 5 
                    		                    mov EDX, EBP 
                    sub EDX, 4 
                    mov word [EDX], 4 
                                        mov EDX, EBP 
                    sub EDX, 8
                    mov dword [EDX], __?float32?__ (6.26) 
                                    mov EDX, EBP
                sub EDX, 4     ; make EDX to point at location of variable on the stack
                push word [zero] ;
                push word [EDX]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                				;Deallocate space given to I/O variables on stack
				add ESP, 8
				LEAVE

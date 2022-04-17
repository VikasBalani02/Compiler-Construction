extern printf, scanf, exit
section .data
		int_fmt: db "%d", 10, 0
		real_fmt: db "%.2lf", 10, 0
		real_fmt_new: db "%f", 10, 0
		intSCAN: dd 0
		zero: dw 0
		flttmp: dq 0.0
section .text
global main
main:
				ENTER 30, 0
				;reserve space for the input/output params of fn, later flush this space
MOV AX, 3
MOV word [EBP-2], AX
MOV EAX, __?float32?__(24.95)
MOVD XMM3,EAX
MOVSS XMM0, XMM3
MOVSS dword[EBP-6], XMM0
MOV AX, 2
MOV word [EBP-8], AX
MOV EAX, __?float32?__(98.80)
MOVD XMM3,EAX
MOVSS XMM0, XMM3
MOVSS dword[EBP-12], XMM0
				mov AX, word[EBP-2]
                mov BX, word[EBP-8]
                add AX, BX 
                mov word [EBP-26], AX 
				movss XMM0, dword[EBP-6]
                movss XMM1, dword[EBP-12]
                addss XMM0, XMM1 
                movss dword[EBP-30], XMM0 
MOV AX, word[EBP-26]
MOV word [EBP-14], AX
MOVSS XMM0, dword[EBP-30]
MOVSS dword[EBP-18], XMM0
                mov EDX, EBP
                sub EDX, 14     ; make EDX to point at location of variable on the stack
                push word [zero] ;
                push word [EDX]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                                mov EDX, EBP
                sub EDX, 18     ; make EDX to point at location of variable on the stack
                fld	dword [EDX]	            ; need to convert 32-bit to 64-bit 
	            fstp	qword [flttmp]      ; floating load makes 80-bit,
	                                        ; store as 64-bit 
                push dword [flttmp+4]       
                push dword [flttmp]         
                push dword real_fmt 
                call printf 
                add ESP,12 
                				;Deallocate space given to I/O variables on stack
				add ESP, 30
				LEAVE
RET

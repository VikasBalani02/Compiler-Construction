extern printf, scanf, exit
section .data
		int_fmt: db "%d", 10, 0
		real_fmt: db "%.2lf", 10, 0
		zero: dw 0
		flttmp: dq 0.0
section .text
global main
_sum:
				ENTER 8, 0
				;reserve space for the input/output params of fn, later flush this space
MOV AX, word [EBP+8]
MOV word [EBP-2], AX
MOV AX, word [EBP+10]
MOV word [EBP-4], AX
MOV AX, word [EBP+12]
MOV word [EBP-6], AX
				mov AX, word[EBP-2]
                mov BX, word[EBP-4]
                add AX, BX 
                mov word [EBP-8], AX 
MOV AX, word[EBP-8]
MOV word [EBP-6], AX
MOV AX, word [EBP-2]
MOV word [EBP+8], AX
MOV AX, word [EBP-4]
MOV word [EBP+10], AX
MOV AX, word [EBP-6]
MOV word [EBP+12], AX
				;Deallocate space given to I/O variables on stack
				add ESP, 8
				LEAVE
RET
main:
				ENTER 6, 0
				;reserve space for the input/output params of fn, later flush this space
MOV AX, 2
MOV word [EBP-2], AX
MOV AX, 3
MOV word [EBP-4], AX
push word [EBP-6]
push word [EBP-4]
push word [EBP-2]
call _sum
add esp,2
add esp,2
mov eax,0
            mov ax,word[ESP]
            add esp,2
            mov word[EBP-6],ax
                mov EDX, EBP
                sub EDX, 6     ; make EDX to point at location of variable on the stack
                push word [zero] ;
                push word [EDX]  ; for integer, value stored at offset should be passed to printf
                push dword int_fmt
                call printf 
                add ESP, 8
                				;Deallocate space given to I/O variables on stack
				add ESP, 6
				LEAVE
RET

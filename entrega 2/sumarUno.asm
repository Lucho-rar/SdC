; This assembly code file contains a function named asm_main that takes a single argument and adds 1 to it.
; The function uses the FPU instruction set to load the argument into a floating-point register, 
; convert it to an integer, and store it in the 'num' variable.
; Then, it adds 1 to the value stored in 'num' and updates the variable.
; Finally, it cleans up the stack and returns.
section .data
    num dd 0

global asm_main
section .text

asm_main:
    push ebp
    mov ebp, esp
    fld dword [esp + 8]
    fistp dword[num]
    mov eax, [num]
    add eax, 1
    mov [num], eax
    mov esp, ebp
    pop ebp
    ret

;%include "io.inc"

section .data
    c11 dq 0.35
    c12 dq -0.95
    c13 dq 2.7
    c21 dq 3.0
    c22 dq 1.0
    c31 dq 1.0
    c32 dq 2.0

section .text
global f1
f1:
    push ebp
    mov ebp, esp
    
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fld qword[c11]
    fmulp
    fmulp
    fld qword[ebp + 8]
    fld qword[c12]
    fmulp
    fld qword[c13]
    faddp
    faddp
    
    leave
    ret

global f2
f2:
    push ebp
    mov ebp, esp
    
    fld qword[ebp + 8]
    fld qword[c21]
    fmulp
    fld qword[c22]
    faddp
    
    leave
    ret

global f3
f3:
    push ebp
    mov ebp, esp
    
    fld qword[c31]
    fld qword[ebp + 8]
    fld qword[c32]
    faddp
    fdivp
    
    leave
    ret

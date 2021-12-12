bits 64
section .text

; Defines ISRs for interrupts that
; push an error code onto the stack:
%macro DEF_ISR_ERR 1
    isr%1:
        push qword %1 ; Push interrupt vector
        jmp call_interrupt_handler
%endmacro

; Defines ISRs for interrupts that
; do not push an error code:
%macro DEF_ISR_NOERR 1
    isr%1:
        ; Padding, because the error code
        ; won't be pushed by the processor:
        push qword 0 ; 0 = no error
        push qword %1 ; Push interrupt vector
        jmp call_interrupt_handler ; Call the interrupt handler
%endmacro

; "Puts" an ISR's pointer into the executable:
%macro PUT_ISR 1
    dq isr%1
%endmacro

; Define all the ISRs:
DEF_ISR_NOERR 0
DEF_ISR_NOERR 1
DEF_ISR_NOERR 2
DEF_ISR_NOERR 3
DEF_ISR_NOERR 4
DEF_ISR_NOERR 5
DEF_ISR_NOERR 6
DEF_ISR_NOERR 7
DEF_ISR_ERR 8
DEF_ISR_NOERR 9
DEF_ISR_ERR 10
DEF_ISR_ERR 11
DEF_ISR_ERR 12
DEF_ISR_ERR 13
DEF_ISR_ERR 14
DEF_ISR_NOERR 15
DEF_ISR_NOERR 16
DEF_ISR_ERR 17
DEF_ISR_NOERR 18
DEF_ISR_NOERR 19
DEF_ISR_NOERR 20
DEF_ISR_ERR 21
DEF_ISR_NOERR 22
DEF_ISR_NOERR 23
DEF_ISR_NOERR 24
DEF_ISR_NOERR 25
DEF_ISR_NOERR 26
DEF_ISR_NOERR 27
DEF_ISR_NOERR 28
DEF_ISR_NOERR 29
DEF_ISR_ERR 30
DEF_ISR_NOERR 31

; Array of pointers to ISRs:
global isr_pointers
isr_pointers:
    %assign i 0
    %rep 32
        PUT_ISR i
    %assign i i+1
    %endrep

; Calls the interrupt handler with the
; stack pointer as a parameter. This 
; gives the handler access to the error
; code and the interrupt vector pushed
; onto the stack by the ISR.
extern interrupt_handler
call_interrupt_handler:
    ; Clear DFLAG for SysV ABI compatibility:
    cld
    mov rdi, rsp
    call interrupt_handler
    add rsp, 16 ; Pop interrupt vector and error code
    iretq

; Load IDT
global load_idt
load_idt:
    ; rdi register is used to pass the first
    ; parameter in 64 bit System V ABI.
    lidt [rdi]
    ret

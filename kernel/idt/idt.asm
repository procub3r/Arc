bits 64
section .text

global load_idt

; Load IDT
load_idt:
    ; rdi register is used to pass the first
    ; parameter in 64 bit System V ABI.
    lidt [rdi]
    ret

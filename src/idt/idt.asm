section .asm
extern no_interupt_handler
extern int21h_handler

global int21h
global idt_load
global no_interupt

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt[ebx]

    pop ebp
    ret

int21h:
    cli
    pushad
    call int21h_handler
    popad
    sti
    iret

no_interupt:
    cli
    pushad
    call no_interupt_handler
    popad
    sti
    iret

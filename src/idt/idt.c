#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include <stdint.h>
#include "io/io.h"

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interupt();

void int21h_handler()
{
    print("Keyboard pressed\n");
    outb(0x20, 0x20);
}

void no_interupt_handler()
{
    outb(0x20, 0x20);
}

void idt_zero() {
    print("\nDivive by zero error!\n");
}

struct idt_desc idt_descriptor[OS_TOTAL_INTERUPTS];

struct idtr_desc idtr_descriptor;

void idt_set(int interupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptor[interupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero =0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t) address >> 16;

}

void idt_init()
{
    memset(idt_descriptor, 0, sizeof(idt_descriptor));
    idtr_descriptor.limit = sizeof(idt_descriptor) - 1;
    idtr_descriptor.base = (uint32_t) idt_descriptor;

    for (int i = 0; i < OS_TOTAL_INTERUPTS; i++)
    {
        idt_set(i, no_interupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    idt_load(&idtr_descriptor);
}

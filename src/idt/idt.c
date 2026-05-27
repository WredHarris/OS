#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include <stdint.h>

extern void idt_load(struct idtr_desc* ptr);

void idt_zero() {
    print("Divive by zero error!\n");
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
    idt_set(32, idt_zero);
    idt_load(&idtr_descriptor);
}

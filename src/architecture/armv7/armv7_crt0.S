// EPOS ARMv7 Program Starter

        .file "armv7_crt0.S"

        // Entry point
        .section .text
        .align  2
        .global irq_stack, svc_stack, _start,CPUsecondary
        .type   _start , function

svc_stack:
.word 0x0FEFFFFC //BOOT_STACK
irq_stack:
.word 0xFF00FFC //BOOT_STACK−0x1000
_start:
// get CPU ID and keep it in R11
 MRC p15, 0, r11, c0, c0, 5 // read CPU ID register into R11
 AND r11, r11, #0x03 // mask in only CPUID
 // set SVC stack
 LDR r0, =svc_stack //__boot_stack__
 mov r1, r11 // r1 = cpuid
 add r1, r1, #1 // cpuid++
 lsl r2, r1, #12 // (cpuid+1)* 4096
 add r0, r0, r2
 mov sp, r0 // SVC sp=svc_stack[cpuid] high end
 // go in IRQ mode with interrupts OFF
 MSR cpsr, #0x92
 // set IRQ stack
 LDR r0, =irq_stack // r0->irq_stack (16KB area in t.ld)
 mov r1, r11
 add r1, r1, #1
 lsl r2, r1, #12 // (cpuid+1) * 4096
 add r0, r0, r2
 mov sp, r0 // IRQ sp=irq_stack[cpuid] high end
 // go back to SVC mode with IRQ ON
 MSR cpsr, #0x13
 cmp r11, #0
 bne CPUsecondary // goto CPUsecondary if not CPU0 
 //bl copy_vectors //copy the vector table to 0x00
// Clear the BSS
        eor     r0, r0
        ldr     r1, =__bss_start__
        ldr     r2, =__bss_end__
.L1:    str     r0, [r1]
        add     r1, #4
        cmp     r1, r2
        blt     .L1

CPUsecondary:// execute by CPUs secondary
        // Call _init
bl  _init
        // Call main
        .align  2
        .globl  __epos_app_entry
        .type   __epos_app_entry, function
__epos_app_entry:
        bl      main
        push    {r0}         // save main's return value to be used by exit()
        bl      _fini
        bl      _exit

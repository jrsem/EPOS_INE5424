// EPOS Cortex-A SETUP
        .file "cortex_a_setup.S" 
        // Interrupt Vector Table
        .section .init
        .type _vector_table, object
        .global _reset, _vector_table, copy_vectors
        .type   _reset,  function

_vector_table:
        ldr pc, _reset_addr
        ldr pc, _undefined_instruction_addr
        ldr pc, _swi_addr
        ldr pc, _prefetch_abort_addr
        ldr pc, _data_abort_addr
        ldr pc, _reserved_addr
        ldr pc, _irq_addr
        ldr pc, _fiq_addr

_reset_addr:
        .word _reset
_undefined_instruction_addr:
        .word _undefined_instruction
_swi_addr:
        .word _software_interrupt
_prefetch_abort_addr:
        .word _prefetch_abort
_data_abort_addr:
        .word _data_abort
_reserved_addr:
        .word _reserved
_irq_addr:
        .word _int_entry
_fiq_addr:
        .word _fiq
svc_stack:
.word 0x0FEFFFFC
irq_stack:
.word 0x0FF02FFC //16kb
 _reset: 
 // get CPU ID and keep it in R11
MRC p15, 0, r11, c0, c0, 5 // read CPU ID register into R11
AND r11, r11, #0x03 // mask in only CPUID
// set SVC stack
LDR r0, =svc_stack // r0->svc_stack (16KB area in t.ld)
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
bne APs // only CPU0 copy vectors, call main()
bl _main

APs:
wfi
bl APs
// EPOS Cortex-M SETUP

#include <system/config.h>

extern "C"
{
  void _vector_table() __attribute__((used, naked, section(".init")));
}

// Interrupt Vector Table
void _vector_table()
{ // LSB must be 1 in thumb mode, so add 1 to all symbols
  ASM("                                             \t\n\
        ldr pc, _start_addr                         \t\n\
        ldr pc, _undefined_instruction_addr         \t\n\
        ldr pc, _swi_addr                           \t\n\
        ldr pc, _prefetch_abort_addr                \t\n\
        ldr pc, _data_abort_addr                    \t\n\
        ldr pc, _reserved_addr                      \t\n\
        ldr pc, _irq_addr                           \t\n\
        ldr pc, _fiq_addr                           \t\n\
       _start_addr:                                 \t\n\
        .word _start                                \t\n\
_undefined_instruction_addr:                        \t\n\
        .word _undefined_instruction                \t\n\
_swi_addr:                                          \t\n\
        .word _software_interrupt                   \t\n\
_prefetch_abort_addr:                               \t\n\
        .word _prefetch_abort                       \t\n\
_data_abort_addr:                                   \t\n\
        .word _data_abort                           \t\n\
_reserved_addr:                                     \t\n\
        .word _reserved                             \t\n\
_irq_addr:                                          \t\n\
        .word _int_entry                            \t\n\
_fiq_addr:                                          \t\n\
        .word _fiq                                  \t\n\
        ");
}
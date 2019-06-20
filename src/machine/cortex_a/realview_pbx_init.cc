// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>
#include <machine.h> //Display::init()
#ifdef __mmod_realview_pbx__

void _startup_cache();

__BEGIN_SYS

void RealView_PBX::enableSCU()
{
  //enable the SCU(Snoop Control Unit )
  //The SCU ensures L1 cache coherence among the CPUs
  // SMP utility functions:
  asm("\t\n\
MRC p15, 4, r0, c15, c0, 0 // Read periph base address\t\n\
LDR r1, [r0, #0x0] // Read the SCU Control Register\t\n\
ORR r1, r1, #0x1 // Set bit 0 (The Enable bit)\t\n\
STR r1, [r0, #0x0] // Write back modifed value\t\n\
BX lr");
}
void RealView_PBX::copyVectorTable()
{
  //copy the vector table in RAM
  ASM("mcr p15, 0, %0, c12, c0, 0"
      :
      : "p"(Traits<Machine>::VECTOR_TABLE));
}
void RealView_PBX::pre_init()
{
}

void RealView_PBX::init()
{
}

__END_SYS

#endif

// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>
#include <machine.h> //Display::init()
#ifdef __mmod_realview_pbx__

//void _startup_cache();

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

void RealView_PBX::pre_init()
{
}

void RealView_PBX::init()
{
  config_gic();
}

void RealView_PBX::config_int(int intID, int targetCPU)
{
  int reg_offset, index, address;
  char priority = 0x80;
  reg_offset = (intID >> 3) & 0xFFFFFFFC;
  index = intID & 0x1F;
  address = (GIC_BASE + 0x1100) + reg_offset;
  *(int *)address = (1 << index);
  // set interrupt ID priority
  reg_offset = (intID & 0xFFFFFFFC);
  index = intID & 0x3;
  address = (GIC_BASE + 0x1400) + reg_offset + index;
  *(char *)address = (char)priority;
  // set target CPUs
  address = (GIC_BASE + 0x1800) + reg_offset + index;
  *(char *)address = (char)(1 << targetCPU);
}
void RealView_PBX::config_gic()
{
  // set int priority mask register
  *(int *)(GIC_BASE + 0x104) = 0xFF;
  // Enable CPU interface control register to signal interrupts
  *(int *)(GIC_BASE + 0x100) = 1;
  // Enable distributor control register to send interrupts to CPUs
  *(int *)(GIC_BASE + 0x1000) = 1;
  // config_int(36, 0); // timer ID=36 to CPU0
  // config_int(44, 1); // UART0 ID=44 to CPU1
}
__END_SYS

#endif

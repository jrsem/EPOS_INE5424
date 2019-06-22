// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>
#include <machine.h> //Display::init()
#ifdef __mmod_realview_pbx__

//void _startup_cache();

__BEGIN_SYS

void RealView_PBX::pre_init()
{
  ASM("mcr p15, 0, %0, c12, c0, 0"
      :
      : "p"(Traits<Machine>::VECTOR_TABLE));
}

void RealView_PBX::init()
{
  Machine::enable_GIC();
  Machine::enable_gic_processor_interface();
  Machine::send_sgi(0x00, 0x0f, 0x01);
  int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
  *apAddr = (int)0x10000;          // all APs execute from 0x10000
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
  config_int(36, 0); // timer ID=36 to CPU0
                     // config_int(44, 1); // UART0 ID=44 to CPU1
}
__END_SYS

#endif

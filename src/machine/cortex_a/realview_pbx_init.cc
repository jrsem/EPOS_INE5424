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

  Machine::join_smp();
  Machine::enable_maintenance_broadcast();
  // Machine::n_cpus();
  Machine::enable_gic_processor_interface();

  if (cpu_id() == 0)
  {
    Machine::secure_SCU_invalidate(Machine::cpu_id(), 0xFb1111);
    Machine::enable_scu();
    Machine::enable_GIC();
    int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
    *apAddr = (int)0x10000;          // all APs execute from 0x10000
    Machine::send_sgi(0x00, 0x0f, 0x01);
  }
}

void RealView_PBX::init()
{
}
__END_SYS

#endif

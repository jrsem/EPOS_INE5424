// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>
#include <machine.h> //Display::init()
#ifdef __mmod_realview_pbx__

//void _startup_cache();

__BEGIN_SYS

void RealView_PBX::pre_init()
{
}

void RealView_PBX::init()
{
  ASM("mcr p15, 0, %0, c12, c0, 0"
      :
      : "p"(Traits<Machine>::VECTOR_TABLE));
  Machine::enable_scu();
  Machine::secure_SCU_invalidate(Machine::cpu_id(), unsigned int ways);
  Machine::join_smp();
  Machine::enable_maintenance_broadcast();
  Machine::n_cpus();

  if (cpu_id() == 0)
  {
    Machine::enable_GIC();
    Machine::enable_gic_processor_interface();
    int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
    *apAddr = (int)0x10000;          // all APs execute from 0x10000
    Machine::send_sgi(0x00, 0x0f, 0x01);
  }
}
__END_SYS

#endif

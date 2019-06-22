// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>
#include <machine.h>             //Display::init()
#include <machine/cortex_a/ic.h> //enable_gic_processor_interface();enable_GIC()
#ifdef __mmod_realview_pbx__

//void _startup_cache();

__BEGIN_SYS

void RealView_PBX::pre_init()
{
  //realocate the vector table
  ASM("mcr p15, 0, %0, c12, c0, 0"
      :
      : "p"(Traits<Machine>::VECTOR_TABLE));

  //CPU join the amp
  Machine::join_smp();

  //enable bredcast
  Machine::enable_maintenance_broadcast();

  //enable gic interface
  enable_gic_processor_interface();

  //only cpu0
  if (cpu_id() == 0)
  {
    Machine::secure_SCU_invalidate(Machine::cpu_id(), 0xFb1111);
    Machine::enable_scu();
    GIC::init();

    //send start address for secundary CPUs
    int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
    *apAddr = (int)0x10000;          // all APs execute from 0x10000

    //send sgi
    Machine::send_sgi(0x00, 0x0f, 0x01);
  }
}

void RealView_PBX::init()
{
}
__END_SYS

#endif

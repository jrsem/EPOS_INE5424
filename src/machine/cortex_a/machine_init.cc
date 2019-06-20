// EPOS Cortex-A Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void Machine::pre_init(System_Info *si)
{
    if (cpu_id == 0)
    {
        Machine_Model::copyVectorTable();
        Display::init();
        Machine_Model::enableSCU();
    }
}
void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;
    Machine_Model::init();
    if (Traits<IC>::enabled)
        IC::init();
    if (Traits<Timer>::enabled)
        Timer::init();
#ifdef __USB_H
    if (Traits<USB>::enabled)
        USB::init();
#endif
}

//SGI method
void Machine::send_sgi(int intID, int targetCPU, int filter)
{
    int *sgi_reg = (int *)(0x1f000000 + 0x1F00);
    *sgi_reg = (filter << 24) | ((1 << targetCPU) << 16) | (intID);
}

//===================================================
//Send SGIs to wakeup other CPUs from the WFI state
//intID=0x00, CPUID = 0 to 3, filter=0x00
//intID=0x00, CPUID = 1 to 3, filter=0x00
//intID=0x00, CPUID = 2 to 3, filter=0x00
//intID=0x00, CPUID = 3 to 3, filter=0x00

//Send SGI to all
//intID=0x00, CPUID = 0x0f to 3, filter=0x01
//send_sgi(0x00, CPUID, 0x01);
//====================================================
// send_sgi(0x0, 0x0f, 0x01); //DEU UM SGI NO CPU1
// int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
// *apAddr = (int)0x10000;          // all APs execute from 0x10000
//====================================================

__END_SYS

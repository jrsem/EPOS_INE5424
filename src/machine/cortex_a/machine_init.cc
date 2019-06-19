// EPOS Cortex-A Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void Machine::pre_init(System_Info *si)
{
    Machine_Model::pre_init();

    Display::init();
}

void Machine::send_sgi(int intID, int targetCPU, int filter)
{
    int *sgi_reg = (int *)(0x1f000000 + 0x1F00);
    *sgi_reg = (filter << 24) | ((1 << targetCPU) << 16) | (intID);
}

void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;
    //===============
    // for (int i = 0; i <= 2; i++)
    Machine::smp_barrier();
    // if (Machine::cpu_id() == 0)
    // {
    send_sgi(0x0, 0x0f, 0x01); //DEU UM SGI NO CPU1
    // int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
    // *apAddr = (int)0x10000;          // all APs execute from 0x10000
    // }

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

__END_SYS

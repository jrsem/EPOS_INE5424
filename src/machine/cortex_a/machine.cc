// EPOS Cortex-A Mediator Implementation

#include <machine/cortex_a/machine.h>
#include <machine/display.h>

__BEGIN_SYS
volatile unsigned int Machine::_n_cpus; //contador de cpus
void Machine::panic()
{
    CPU::int_disable();
    if (Traits<Display>::enabled)
        Display::puts("PANIC!\n");
    if (Traits<System>::reboot)
        reboot();
    else
        CPU::halt();
}

void Machine::reboot()
{
    db<Machine>(WRN) << "Machine::reboot()" << endl;
    Machine_Model::reboot();
}
void Machine::enable_GIC(void)
{
    // Global enable of the Interrupt Distributor
    /* enable_GIC PROC */

    // Get base address of private perpherial space
    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address
    __asm("ADD     r0, r0, #0x1000");        // Add the GIC offset

    __asm("LDR     r1, [r0]");      // Read the GIC's Enable Register  (ICDDCR)
    __asm("ORR     r1, r1, #0x01"); // Set bit 0, the enable bit
    __asm("STR     r1, [r0]");      // Write the GIC's Enable Register  (ICDDCR)

    __asm("BX      lr");
    /* ENDP */
}
void Machine::send_sgi(int intID, int targetCPU, int filter)
{
    // Send a software generate interrupt
    /* send_sgi PROC */

    __asm("AND     r3, r0, #0x0F"); // Mask off unused bits of ID, and move to r3
    __asm("AND     r1, r1, #0x0F"); // Mask off unused bits of target_filter
    __asm("AND     r2, r2, #0x0F"); // Mask off unused bits of filter_list

    __asm("ORR     r3, r3, r1, LSL #16"); // Combine ID and target_filter
    __asm("ORR     r3, r3, r2, LSL #24"); // and now the filter list

    // Get the address of the GIC
    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address
    __asm("ADD     r0, r0, #0x1F00");        // Add offset of the sgi_trigger reg

    __asm("STR     r3, [r0]"); // Write to the Software Generated Interrupt Register  (ICDSGIR)

    __asm("BX      lr");
    /* ENDP */
}

void Machine::disable_gic_processor_interface(void)
{
    // Disables the processor interface
    // Must been done one each CPU seperately
    /* disable_gic_processor_interface PROC */

    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address

    __asm("LDR     r1, [r0, #0x100]"); // Read the Processor Interface Control register   (ICCICR/ICPICR)
    __asm("BIC     r1, r1, #0x03");    // Bit 0: Enables secure interrupts, Bit 1: Enables Non-Secure interrupts
    __asm("STR     r1, [r0, #0x100]"); // Write the Processor Interface Control register  (ICCICR/ICPICR)

    __asm("BX      lr");
}
void Machine::disable_GIC(void)
{
    // Global disable of the Interrupt Distributor
    /* disable_GIC PROC */

    // Get base address of private perpherial space
    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address
    __asm("ADD     r0, r0, #0x1000");        // Add the GIC offset

    __asm("LDR     r1, [r0]");      // Read the GIC's Enable Register  (ICDDCR)
    __asm("BIC     r1, r1, #0x01"); // Set bit 0, the enable bit
    __asm("STR     r1, [r0]");      // Write the GIC's Enable Register  (ICDDCR)

    __asm("BX      lr");
    /* ENDP */
}
void Machine::enable_gic_processor_interface(void)
{
    // Enables the processor interface
    // Must been done one each CPU seperately
    /* enable_gic_processor_interface PROC */

    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address

    __asm("LDR     r1, [r0, #0x100]"); // Read the Processor Interface Control register   (ICCICR/ICPICR)
    __asm("ORR     r1, r1, #0x03");    // Bit 0: Enables secure interrupts, Bit 1: Enables Non-Secure interrupts
    __asm("STR     r1, [r0, #0x100]"); // Write the Processor Interface Control register  (ICCICR/ICPICR)

    __asm("BX      lr");
    /* ENDP */
}
void Machine::enable_scu()
{
    // Enables the SCU
    /* enable_scu PROC */

    __asm("MRC     p15, 4, r0, c15, c0, 0"); // Read periph base address

    __asm("LDR     r1, [r0, #0x0]"); // Read the SCU Control Register
    __asm("ORR     r1, r1, #0x1");   // Set bit 0 (The Enable bit)
    __asm("STR     r1, [r0, #0x0]"); // Write back modifed value

    __asm("BX      lr");
    /* ENDP */
}
void Machine::join_smp(void)
{
    // Set this CPU as participating in SMP
    /* join_smp  PROC */

    // SMP status is controlled by bit 6 of the CP15 Aux Ctrl Reg

    __asm("MRC     p15, 0, r0, c1, c0, 1"); // Read ACTLR
    __asm("ORR     r0, r0, #0x040");        // Set bit 6
    __asm("MCR     p15, 0, r0, c1, c0, 1"); // Write ACTLR

    __asm("BX      lr");
    /* ENDP */
}
void Machine::smp_barrier(unsigned long n_cpus)
{
    static volatile unsigned long ready[2];
    static volatile unsigned long i;

    if (smp)
    {
        int j = i;

        CPU::finc(ready[j]);
        if (cpu_id() == 0)
        {
            while (ready[j] < n_cpus)
                ;         // wait for all CPUs to be ready
            i = !i;       // toggle ready
            ready[j] = 0; // signalizes waiting CPUs
        }
        else
        {
            while (ready[j])
                ; // wait for CPU[0] signal
        }
    }
}
__END_SYS

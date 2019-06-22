extern "C"
{
  // static unsigned int cpu_id()
  // {
  //   int id;
  //   asm("mrc p15, 0, %0, c0, c0, 5"
  //       : "=r"(id)
  //       :
  //       :);
  //   return id & 0x3;
  // }

#define GIC_BASE 0x1F000000
  int *apAddr = (int *)0x10000030; // SYS_FLAGSSET register
                                   // same as before
  void APstart()                   // AP startup code
  {
    //int cpuid = cpu_id();
    while (1)
    {
      asm("WFI");
    }
  }
  //==
  void config_int(int intID, int targetCPU)
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

  //===
  void config_gic()
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
  //===
}
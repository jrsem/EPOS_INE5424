// EPOS LM3S811 (ARM Cortex-A9) MCU Metainfo and Configuration

#ifndef __machine_traits_h
#define __machine_traits_h

#include <system/config.h>

__BEGIN_SYS

class Machine_Common;
template <>
struct Traits<Machine_Common> : public Traits<void>
{
  static const bool debugged = Traits<void>::debugged;
};

//configuração da maquina realview
template <>
struct Traits<Machine> : public Traits<Machine_Common>
{
  static const unsigned int CPUS = Traits<Build>::CPUS;
  // Physical Memory
  static const unsigned int MEM_BASE = 0x00000000; //o vetor de tabela é copiado para o endereço 0x00
  static const unsigned int MEM_TOP = 0x07FFFFFc;  // (0x10000000-4 ==>ultimo endereço da memoria)=== 4GB

  static const unsigned int BOOT_STACK = 0x07eFFFFC; //(0x07F00000 - 4) // MEM_TOP - sizeof(int)

  static const unsigned int APP_LOW = 0x000000000;

  static const unsigned int APP_CODE = 0x00010000;

  static const unsigned int APP_DATA = 0x00010000;
  static const unsigned int APP_HIGH = 0x07FFFFFc; // 8 KB

  static const unsigned int PHY_MEM = 0x80000000; //2GB

  //OI :UARTs/TIMERs
  static const unsigned int IO_BASE = 0xf0000000;
  static const unsigned int IO_TOP = 0xFF3FFFFC; //(0xff400000-4) ultimo endereço do IO

  //os s do sistema
  static const unsigned int SYS = 0xFF3FFFFC;      //endereço base do systema
  static const unsigned int SYS_CODE = 0xFF700000; // Library mode only => APP + SYS
  static const unsigned int SYS_DATA = 0xFF740000; // Library mode only => APP + SYS

  // Default Sizes and Quantities
  //tamanho dos heap e stack do realview-pbx
  //max_thread para que pode ser executado no realview-pbx;
  static const unsigned int STACK_SIZE = 65536; //atribuimos toda o SRAM ao stack
  static const unsigned int HEAP_SIZE = 65536;  //atribuimos toda o SRAM ao heap
  static const unsigned int MAX_THREADS = 16;   //2;  //com "1" deu erro out of mamory (2 thread "o do hello worl" e o do "idle")
};

//configuração do IC(controle de interrrupção) da maquina
template <>
struct Traits<IC> : public Traits<Machine_Common>
{
  static const bool debugged = hysterically_debugged;
};

//configuração do Timer da maquina
template <>
struct Traits<Timer> : public Traits<Machine_Common>
{
  static const bool debugged = hysterically_debugged;

  // Meaningful values for the timer frequency range from 100 to
  // 10000 Hz. The choice must respect the scheduler time-slice, i. e.,
  // it must be higher than the scheduler invocation frequency.
  static const int FREQUENCY = 1000; // Hz  //a frequencia do realview-pbx
};

template <>

//configuração do UART do realview-pbx
struct Traits<UART> : public Traits<Machine_Common>
{
  static const unsigned int UNITS = 2; //UART0 e UART1

  static const unsigned int CLOCK = Traits<CPU>::CLOCK;

  static const unsigned int DEF_UNIT = 1;
  static const unsigned int DEF_BAUD_RATE = 115200;
  static const unsigned int DEF_DATA_BITS = 8;
  static const unsigned int DEF_PARITY = 0; // none
  static const unsigned int DEF_STOP_BITS = 1;
};

//

// //configuração do USB da maquina
// template <>
// struct Traits<USB> : public Traits<Machine_Common>
// {
//   static const bool enabled = false;
//   static const unsigned int UNITS = 0;
//   static const bool blocking = true;
// };

// template <>
// struct Traits<Scratchpad> : public Traits<Machine_Common>
// {
//   static const bool enabled = false;
// };

__END_SYS

#endif

// EPOS RealView PBX (ARM Cortex) MCU Initialization

#include <machine/main.h>

#ifdef __mmod_realview_pbx__

void _startup_cache();

__BEGIN_SYS

void RealView_PBX::pre_init() {}

void RealView_PBX::init() {
}

__END_SYS

#endif
